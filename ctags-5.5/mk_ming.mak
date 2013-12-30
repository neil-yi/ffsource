# $Id: mk_ming.mak,v 1.1 2001/11/02 04:53:43 darren Exp $
#
# Makefile for Exuberant Ctags under Win32 with Mingw32 compiler
#
# Note that only the crtdll variant of the compiler is supported.
# The msvcrt variant has broken implementations of fgetpos() and fsetpos().

include source.mak

CFLAGS = -Wall
DEFINES = -DWIN32
CC = gcc
DLLFLAGS = -shared -static-libgcc -Wl,--kill-at,--output-def,ctags.def -L. -lsqlite3

#ctags: ctags.exe
ctags: ctags.dll

ctags.dll: OPT = -O4
ctags.dll: LDFLAGS = -s

ctags.exe: OPT = -O4
ctags.exe: LDFLAGS = -s
dctags.exe: OPT = -g

#ctags.exe dctags.exe: $(SOURCES)
#	$(CC) $(LDFLAGS) $(OPT) $(CFLAGS) $(DEFINES)  -o $@ $(SOURCES)

ctags.dll: $(SOURCES)
	$(CC) $(LDFLAGS) $(OPT) $(CFLAGS) $(DEFINES) $(DLLFLAGS) -o ../binU/$@ $(SOURCES)

clean:
	- del ctags.exe
	- del dctags.exe
#	- del tags
	- del ctags.dll
