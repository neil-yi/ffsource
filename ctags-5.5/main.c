/*
*   $Id: main.c,v 1.15 2003/04/01 04:55:27 darren Exp $
*
*   Copyright (c) 1996-2003, Darren Hiebert
*
*   Author: Darren Hiebert <dhiebert@users.sourceforge.net>
*           http://ctags.sourceforge.net
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License. It is provided on an as-is basis and no
*   responsibility is accepted for its failure to perform as expected.
*
*   This is a reimplementation of the ctags (1) program. It is an attempt to
*   provide a fully featured ctags program which is free of the limitations
*   which most (all?) others are subject to.
*
*   This module contains the start-up code and routines to determine the list
*   of files to parsed for tags.
*/

/*
*   INCLUDE FILES
*/
#include "general.h"	/* must always come first */

#include <string.h>
#include "sqlite3.h"

/*  To provide timings features if available.
 */
#ifdef HAVE_CLOCK
# ifdef HAVE_TIME_H
#  include <time.h>
# endif
#else
# ifdef HAVE_TIMES
#  ifdef HAVE_SYS_TIMES_H
#   include <sys/times.h>
#  endif
# endif
#endif

/*  To provide directory searching for recursion feature.
 */
#ifdef AMIGA
# include <dos/dosasl.h>	/* for struct AnchorPath */
# include <clib/dos_protos.h>	/* function prototypes */
# define ANCHOR_BUF_SIZE 512
# define ANCHOR_SIZE (sizeof (struct AnchorPath) + ANCHOR_BUF_SIZE)
# ifdef __SASC
   extern struct DosLibrary *DOSBase;
#  include <pragmas/dos_pragmas.h>
# endif
#endif

#ifdef HAVE_DIRENT_H
# ifdef __BORLANDC__
#  define boolean BORLAND_boolean
# endif
# ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>    /* required by dirent.h */
# endif
# include <dirent.h>	/* to declare opendir() */
# undef boolean
#endif
#ifdef HAVE_DIRECT_H
# include <direct.h>	/* to _getcwd */
#endif
#ifdef HAVE_DOS_H
# include <dos.h>	/* to declare FA_DIREC */
#endif
#ifdef HAVE_DIR_H
# include <dir.h>	/* to declare findfirst () and findnext () */
#endif
#ifdef HAVE_IO_H
# include <io.h>	/* to declare _finddata_t in MSVC++ 4.x */
#endif


#include "debug.h"
#include "keyword.h"
#include "main.h"
#include "options.h"
#include "read.h"
#include "routines.h"

/*
*   MACROS
*/
#define plural(value)		(((unsigned long)(value) == 1L) ? "" : "s")

/*
*   DATA DEFINITIONS
*/
static stringList* Excluded = NULL;
static struct { long files, lines, bytes; } Totals = { 0, 0, 0 };

#ifdef AMIGA
# include "ctags.h"
  static const char *VERsion = "$VER: "PROGRAM_NAME" "PROGRAM_VERSION" "
# ifdef __SASC
  __AMIGADATE__
# else
  __DATE__
# endif
  " "AUTHOR_NAME" $";
#endif

/*
*   FUNCTION PROTOTYPES
*/
static boolean createTagsForEntry (const char *const entryName);

/*
*   FUNCTION DEFINITIONS
*/

extern vString *combinePathAndFile (
    const char *const path, const char *const file)
{
    vString *const filePath = vStringNew ();
#ifdef VMS
    const char *const directoryId = strstr (file, ".DIR;1");

    if (directoryId == NULL)
    {
	const char *const versionId = strchr (file, ';');

	vStringCopyS (filePath, path);
	if (versionId == NULL)
	    vStringCatS (filePath, file);
	else
	    vStringNCatS (filePath, file, versionId - file);
	vStringCopyToLower (filePath, filePath);
    }
    else
    {
	/*  File really is a directory; append it to the path.
	 *  Gotcha: doesn't work with logical names.
	 */
	vStringNCopyS (filePath, path, strlen (path) - 1);
	vStringPut (filePath, '.');
	vStringNCatS (filePath, file, directoryId - file);
	if (strchr (path, '[') != NULL)
	    vStringPut (filePath, ']');
	else
	    vStringPut (filePath, '>');
	vStringTerminate (filePath);
    }
#else
    const int lastChar = path [strlen (path) - 1];
# ifdef MSDOS_STYLE_PATH
    boolean terminated = (boolean) (strchr (PathDelimiters, lastChar) != NULL);
# else
    boolean terminated = (boolean) (lastChar == PATH_SEPARATOR);
# endif

    vStringCopyS (filePath, path);
    if (! terminated)
    {
	vStringPut (filePath, OUTPUT_PATH_SEPARATOR);
	vStringTerminate (filePath);
    }
    vStringCatS (filePath, file);
#endif

    return filePath;
}

extern void addTotals (
	const unsigned int files, const long unsigned int lines,
	const long unsigned int bytes)
{
    Totals.files += files;
    Totals.lines += lines;
    Totals.bytes += bytes;
}

extern boolean isDestinationStdout (void)
{
    boolean toStdout = FALSE;

    if (Option.xref  ||  Option.filter  ||
	(Option.tagFileName != NULL  &&  (strcmp (Option.tagFileName, "-") == 0
#if defined (VMS)
    || strcmp (Option.tagFileName, "sys$output") == 0
#else
    || strcmp (Option.tagFileName, "/dev/stdout") == 0
#endif
	)))
	toStdout = TRUE;
    return toStdout;
}

extern void processExcludeOption (
	const char *const __unused__ option, const char *const parameter)
{
    const char *const fileName = parameter + 1;
    if (parameter [0] == '\0')
	freeList (&Excluded);
    else if (parameter [0] == '@')
    {
	stringList* const sl = stringListNewFromFile (fileName);
	if (sl == NULL)
	    error (FATAL | PERROR, "cannot open \"%s\"", fileName);
	if (Excluded == NULL)
	    Excluded = sl;
	else
	    stringListCombine (Excluded, sl);
	verbose ("    adding exclude patterns from %s\n", fileName);
    }
    else
    {
	vString *const item = vStringNewInit (parameter);
	if (Excluded == NULL)
	    Excluded = stringListNew ();
	stringListAdd (Excluded, item);
	verbose ("    adding exclude pattern: %s\n", parameter);
    }
}

static boolean excludedFile (const char* const name)
{
    const char* base = baseFilename (name);
    boolean result = FALSE;
    if (Excluded != NULL)
    {
	result = stringListFileMatched (Excluded, base);
	if (! result  &&  name != base)
	    result = stringListFileMatched (Excluded, name);
    }
#ifdef AMIGA
    /* not a good solution, but the only one which works often */
    if (! result)
	result = (boolean) (strcmp (name, TagFile.name) == 0);
#endif
    return result;
}

#if defined (HAVE_OPENDIR)
static boolean recurseUsingOpendir (const char *const dirName)
{
    boolean resize = FALSE;
    DIR *const dir = opendir (dirName);
    if (dir == NULL)
	error (WARNING | PERROR, "cannot recurse into directory \"%s\"", dirName);
    else
    {
	struct dirent *entry;
	while ((entry = readdir (dir)) != NULL)
	{
	    if (strcmp (entry->d_name, ".") != 0  &&
		strcmp (entry->d_name, "..") != 0)
	    {
		vString *filePath;
		if (strcmp (dirName, ".") == 0)
		    filePath = vStringNewInit (entry->d_name);
		else
		    filePath = combinePathAndFile (dirName, entry->d_name);
		resize |= createTagsForEntry (vStringValue (filePath));
		vStringDelete (filePath);
	    }
	}
	closedir (dir);
    }
    return resize;
}

#elif defined (HAVE_FINDFIRST) || defined (HAVE__FINDFIRST)

static boolean createTagsForWildcardEntry (
	const char *const pattern, const size_t dirLength,
	const char *const entryName)
{
    boolean resize = FALSE;
    /* we must not recurse into the directories "." or ".." */
    if (strcmp (entryName, ".") != 0  &&  strcmp (entryName, "..") != 0)
    {
	vString *const filePath = vStringNew ();
	vStringNCopyS (filePath, pattern, dirLength);
	vStringCatS (filePath, entryName);
	resize = createTagsForEntry (vStringValue (filePath));
	vStringDelete (filePath);
    }
    return resize;
}

static boolean createTagsForWildcardUsingFindfirst (const char *const pattern)
{
    boolean resize = FALSE;
    const size_t dirLength = baseFilename (pattern) - pattern;
#if defined (HAVE_FINDFIRST)
    struct ffblk fileInfo;
    int result = findfirst (pattern, &fileInfo, FA_DIREC);
    while (result == 0)
    {
	const char *const entry = (const char *) fileInfo.ff_name;
	resize |= createTagsForWildcardEntry (pattern, dirLength, entry);
	result = findnext (&fileInfo);
    }
#elif defined (HAVE__FINDFIRST)
    struct _finddata_t fileInfo;
    intptr_t hFile = _findfirst (pattern, &fileInfo);
    if (hFile != -1L)
    {
	do
	{
	    const char *const entry = (const char *) fileInfo.name;
	    resize |= createTagsForWildcardEntry (pattern, dirLength, entry);
	} while (_findnext (hFile, &fileInfo) == 0);
	_findclose (hFile);
    }
#endif
    return resize;
}

#elif defined (AMIGA)

static boolean createTagsForAmigaWildcard (const char *const pattern)
{
    boolean resize = FALSE;
    struct AnchorPath *const anchor =
	    (struct AnchorPath *) eMalloc ((size_t) ANCHOR_SIZE);
    LONG result;

    memset (anchor, 0, (size_t) ANCHOR_SIZE);
    anchor->ap_Strlen = ANCHOR_BUF_SIZE;
    /* Allow '.' for current directory */
#ifdef APF_DODOT
    anchor->ap_Flags = APF_DODOT | APF_DOWILD;
#else
    anchor->ap_Flags = APF_DoDot | APF_DoWild;
#endif
    result = MatchFirst ((UBYTE *) pattern, anchor);
    while (result == 0)
    {
	resize |= createTagsForEntry ((char *) anchor->ap_Buf);
	result = MatchNext (anchor);
    }
    MatchEnd (anchor);
    eFree (anchor);
    return resize;
}
#endif

static boolean recurseIntoDirectory (const char *const dirName)
{
    boolean resize = FALSE;
    if (isRecursiveLink (dirName))
	verbose ("ignoring \"%s\" (recursive link)\n", dirName);
    else if (! Option.recurse)
	verbose ("ignoring \"%s\" (directory)\n", dirName);
    else
    {
	verbose ("RECURSING into directory \"%s\"\n", dirName);
#if defined (HAVE_OPENDIR)
	resize = recurseUsingOpendir (dirName);
#elif defined (HAVE_FINDFIRST) || defined (HAVE__FINDFIRST)
	{
	    vString *const pattern = vStringNew ();
	    vStringCopyS (pattern, dirName);
	    vStringPut (pattern, OUTPUT_PATH_SEPARATOR);
	    vStringCatS (pattern, "*.*");
	    resize = createTagsForWildcardUsingFindfirst (vStringValue (pattern));
	    vStringDelete (pattern);
	}
#elif defined (AMIGA)
	{
	    vString *const pattern = vStringNew ();
	    if (*dirName != '\0'  &&  strcmp (dirName, ".") != 0)
	    {
		vStringCopyS (pattern, dirName);
		if (dirName [strlen (dirName) - 1] != '/')
		    vStringPut (pattern, '/');
	    }
	    vStringCatS (pattern, "#?");
	    resize = createTagsForAmigaWildcard (vStringValue (pattern));
	    vStringDelete (pattern);
	}
#endif
    }
    return resize;
}

static boolean createTagsForEntry (const char *const entryName)
{
    boolean resize = FALSE;
    fileStatus *status = eStat (entryName);

    Assert (entryName != NULL);
    if (excludedFile (entryName))
	verbose ("excluding \"%s\"\n", entryName);
    else if (status->isSymbolicLink  &&  ! Option.followLinks)
	verbose ("ignoring \"%s\" (symbolic link)\n", entryName);
    else if (! status->exists)
	error (WARNING | PERROR, "cannot open source file \"%s\"", entryName);
    else if (status->isDirectory)
	resize = recurseIntoDirectory (entryName);
    else if (! status->isNormalFile)
	verbose ("ignoring \"%s\" (special file)\n", entryName);
    else
	resize = parseFile (entryName);

    return resize;
}

#ifdef MANUAL_GLOBBING

static boolean createTagsForWildcardArg (const char *const arg)
{
    boolean resize = FALSE;
    vString *const pattern = vStringNewInit (arg);
    char *patternS = vStringValue (pattern);

#if defined (HAVE_FINDFIRST) || defined (HAVE__FINDFIRST)
    /*  We must transform the "." and ".." forms into something that can
     *  be expanded by the findfirst/_findfirst functions.
     */
    if (Option.recurse  &&
	(strcmp (patternS, ".") == 0  ||  strcmp (patternS, "..") == 0))
    {
	vStringPut (pattern, OUTPUT_PATH_SEPARATOR);
	vStringCatS (pattern, "*.*");
    }
    resize |= createTagsForWildcardUsingFindfirst (patternS);
#endif
    vStringDelete (pattern);
    return resize;
}

#endif

static boolean createTagsForArgs (cookedArgs* const args)
{
    boolean resize = FALSE;

    /*  Generate tags for each argument on the command line.
     */
    while (! cArgOff (args))
    {
	const char *const arg = cArgItem (args);

#ifdef MANUAL_GLOBBING
	resize |= createTagsForWildcardArg (arg);
#else
	resize |= createTagsForEntry (arg);
#endif
	cArgForth (args);
	parseOptions (args);
    }
    return resize;
}

/*  Read from an opened file a list of file names for which to generate tags.
 */
static boolean createTagsFromFileInput (FILE* const fp, const boolean filter)
{
    boolean resize = FALSE;
    if (fp != NULL)
    {
	cookedArgs* args = cArgNewFromLineFile (fp);
	parseOptions (args);
	while (! cArgOff (args))
	{
	    resize |= createTagsForEntry (cArgItem (args));
	    if (filter)
	    {
		if (Option.filterTerminator != NULL)
		    fputs (Option.filterTerminator, stdout);
		fflush (stdout);
	    }
	    cArgForth (args);
	    parseOptions (args);
	}
	cArgDelete (args);
    }
    return resize;
}

/*  Read from a named file a list of file names for which to generate tags.
 */
static boolean createTagsFromListFile (const char* const fileName)
{
    boolean resize;
    Assert (fileName != NULL);
    if (strcmp (fileName, "-") == 0)
	resize = createTagsFromFileInput (stdin, FALSE);
    else
    {
	FILE* const fp = fopen (fileName, "r");
	if (fp == NULL)
	    error (FATAL | PERROR, "cannot open list file \"%s\"", fileName);
	resize = createTagsFromFileInput (fp, FALSE);
	fclose (fp);
    }
    return resize;
}

#if defined (HAVE_CLOCK)
# define CLOCK_AVAILABLE
# ifndef CLOCKS_PER_SEC
#  define CLOCKS_PER_SEC	1000000
# endif
#elif defined (HAVE_TIMES)
# define CLOCK_AVAILABLE
# define CLOCKS_PER_SEC	60
static clock_t clock (void)
{
    struct tms buf;

    times (&buf);
    return (buf.tms_utime + buf.tms_stime);
}
#else
# define clock()  (clock_t)0
#endif

static void printTotals (const clock_t *const timeStamps)
{
    const unsigned long totalTags = TagFile.numTags.added +
				    TagFile.numTags.prev;

    fprintf (errout, "%ld file%s, %ld line%s (%ld kB) scanned",
	    Totals.files, plural (Totals.files),
	    Totals.lines, plural (Totals.lines),
	    Totals.bytes/1024L);
#ifdef CLOCK_AVAILABLE
    {
	const double interval = ((double) (timeStamps [1] - timeStamps [0])) /
				CLOCKS_PER_SEC;

	fprintf (errout, " in %.01f seconds", interval);
	if (interval != (double) 0.0)
	    fprintf (errout, " (%lu kB/s)",
		    (unsigned long) (Totals.bytes / interval) / 1024L);
    }
#endif
    fputc ('\n', errout);

    fprintf (errout, "%lu tag%s added to tag file",
	    TagFile.numTags.added, plural (TagFile.numTags.added));
    if (Option.append)
	fprintf (errout, " (now %lu tags)", totalTags);
    fputc ('\n', errout);

    if (totalTags > 0  &&  Option.sorted != SO_UNSORTED)
    {
	fprintf (errout, "%lu tag%s sorted", totalTags, plural (totalTags));
#ifdef CLOCK_AVAILABLE
	fprintf (errout, " in %.02f seconds",
		((double) (timeStamps [2] - timeStamps [1])) / CLOCKS_PER_SEC);
#endif
	fputc ('\n', errout);
    }

#ifdef DEBUG
    fprintf (errout, "longest tag line = %lu\n",
	    (unsigned long) TagFile.max.line);
#endif
}

static void makeTags (cookedArgs* args)
{
    clock_t timeStamps [3];
    boolean resize = FALSE;
    boolean files = (boolean)(! cArgOff (args) || Option.fileList != NULL
			      || Option.filter);

    if (filesRequired ()  && ! files)
	error (FATAL, "No files specified. Try \"%s --help\".",
	       getExecutableName ());

#define timeStamp(n) timeStamps[(n)]=(Option.printTotals ? clock():(clock_t)0)
    if (! Option.filter)
	openTagFile ();

    timeStamp (0);

    if (! cArgOff (args))
    {
	verbose ("Reading command line arguments\n");
	resize = createTagsForArgs (args);
    }
    if (Option.fileList != NULL)
    {
	verbose ("Reading list file\n");
	resize = (boolean) (createTagsFromListFile (Option.fileList) || resize);
    }
    if (Option.filter)
    {
	verbose ("Reading filter input\n");
	resize = (boolean) (createTagsFromFileInput (stdin, TRUE) || resize);
    }
    if (! files  &&  Option.recurse)
	resize = recurseIntoDirectory (".");

    timeStamp (1);

    if (! Option.filter)
	closeTagFile (resize);

    timeStamp (2);

    if (Option.printTotals)
	printTotals (timeStamps);
#undef timeStamp
}

/*
 *	Start up code
 */

//extern "C" {
    __declspec(dllexport) int main(int argc, char **argv);
//}

//int addDB();

int testdb(void);
int testdb2(void);
int testdb3(void);
 
static int _callback_exec(void * notused,int argc, char ** argv, char ** aszColName)
{
    int i;
    for ( i=0; i<argc; i++ )
    {
        printf( "%s = %s/n", aszColName[i], argv[i] == 0 ? "NUL" : argv[i] );
    }
    return 0;
}
 
int testdb(void)
{
    sqlite3 *db=NULL;
    char *zErrMsg = 0;
    int rc;
    //��ָ�������ݿ��ļ�,��������ڽ�����һ��ͬ�������ݿ��ļ�
    rc = sqlite3_open("test1.db", &db);
    if(rc)
    {
        fprintf(stderr, "Can��t open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return (1);
    }
    else
        printf("You have opened a sqlite3 database named test1.db successfully!\n");
 
    //����һ����,����ñ���ڣ��򲻴�������������ʾ��Ϣ���洢��zErrMsg ��
    char *sql = " CREATE TABLE test(ID INTEGER PRIMARY KEY,AGE INTEGER,LEVEL INTEGER,NAME VARCHAR(12),SALARY REAL);" ;
    sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );
 
    //��������
    sql = "INSERT INTO test VALUES(NULL , 1 , 1 , '201205151206', 117.9 );" ;
    sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );
    int nrow = 0, ncolumn = 0; //��ѯ�����������������
    char **azResult; //��ά�����Ž��
 
    //��ѯ����
    sql = "SELECT * FROM test ";
    sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
    int i = 0 ;
    printf( "row:%d column=%d \n" , nrow , ncolumn );
    printf( "\nThe result of querying is : \n" );
    for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )
    {
        printf( "azResult[%d] = %s\n", i , azResult[i] );
    }
 
    //ɾ������
    sql = "DELETE FROM test WHERE AGE = 1 ;" ;
    sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );
 
    //�ͷŵ�azResult ���ڴ�ռ�
    sqlite3_free_table( azResult );
    sqlite3_close(db); //�ر����ݿ�
 
    int c=getchar();
 
    return 0;
}
 
int testdb2(void)
{
    const char * sSQL = "select * from t1;";
    char * pErrMsg = 0;
    int ret = 0;
    sqlite3 * db = 0;
    ret = sqlite3_open("./test2.db", &db);
    if ( ret != SQLITE_OK )
    {
        fprintf(stderr, "Could not open database: %s", sqlite3_errmsg(db));
        exit(1);
    }
 
    printf("Successfully connected to database/n");
    sqlite3_exec( db, sSQL, _callback_exec, 0, &pErrMsg );
 
    if ( ret != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s/n", pErrMsg);
        sqlite3_free(pErrMsg);
    }
 
    sqlite3_close(db);
    db = 0;
 
    return 0;
}
 
int testdb3(void)
{
    const char * sSQL = "select * from t1;";
    char * pErrMsg = 0;
    int ret = 0;
    sqlite3 * db = 0;
    ret = sqlite3_open(":memory:", &db);
    if ( ret != SQLITE_OK )
    {
        fprintf(stderr, "Could not open database: %s", sqlite3_errmsg(db));
        exit(1);
    }
 
    printf("Successfully connected to database/n");
    char *zErrMsg = 0;
    char *sql = " CREATE TABLE test(ID INTEGER PRIMARY KEY,AGE INTEGER,LEVEL INTEGER,NAME VARCHAR(12),SALARY REAL);" ;
    sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );
 
    //��������
    sql = "INSERT INTO test VALUES(NULL , 1 , 1 , '201205151206', 20.9 );" ;
    sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );
    int nrow = 0, ncolumn = 0; //��ѯ�����������������
    char **azResult; //��ά�����Ž��
 
    //��ѯ����
    sql = "SELECT * FROM test ";
    sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
    int i = 0 ;
    printf( "row:%d column=%d \n" , nrow , ncolumn );
    printf( "\nThe result of querying is : \n" );
   
    for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )
    {
        printf( "azResult[%d] = %s\n", i , azResult[i] );
    }
 
    //�ͷŵ�azResult ���ڴ�ռ�
    sqlite3_free_table( azResult );
    sqlite3_close(db); //�ر����ݿ�
 
    return 0;
}

extern int main (int __unused__ argc, char **argv)
{
    cookedArgs *args;
#ifdef VMS
    extern int getredirection (int *ac, char ***av);

    /* do wildcard expansion and I/O redirection */
    getredirection (&argc, &argv);
#endif

#ifdef AMIGA
    /* This program doesn't work when started from the Workbench */
    if (argc == 0)
	exit (1);
#endif

#ifdef __EMX__
    _wildcard (&argc, &argv);	/* expand wildcards in argument list */
#endif

#if defined (macintosh) && BUILD_MPW_TOOL == 0
    argc = ccommand (&argv);
#endif

    testdb();
    //testdb2();
    //testdb3();

    setCurrentDirectory ();
    setExecutableName (*argv++);
    checkRegex ();

    args = cArgNewFromArgv (argv);
    previewFirstOption (args);
    testEtagsInvocation ();
    initializeParsing ();
    initOptions ();
    readOptionConfiguration ();
    verbose ("Reading initial options from command line\n");
    parseOptions (args);
    checkOptions ();
    makeTags (args);

    /*  Clean up.
     */
    eFree (CurrentDirectory);
    freeList (&Excluded);
    cArgDelete (args);
    freeKeywordTable ();
    freeSourceFileResources ();
    freeTagFileResources ();
    freeOptionResources ();
    freeParserResources ();
    freeRegexResources ();

    exit (0);
    return 0;
}

/* vi:set tabstop=8 shiftwidth=4: */
