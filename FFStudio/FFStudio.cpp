// Gui_DevStudio.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FFStudio.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "FFStudioDoc.h"
#include "FFStudioView.h"
#include "mfcieVw.h"
#include "scintilladocview.h"
//#include "setting.h"
#include "mfcieDoc.h"
#include "AddProjectDlg.h"
#include "sqlite3.h"
#include "OpenProjectDlg.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioApp

BEGIN_MESSAGE_MAP(CFFStudioApp, CWinApp)
	//{{AFX_MSG_MAP(CGui_DevStudioApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_PROJECT_ADDPROJECT, &CFFStudioApp::OnProjectAddproject)
	ON_COMMAND(ID_PROJECT_OPENPROJECT, &CFFStudioApp::OnProjectOpenproject)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioApp construction

CFFStudioApp::CFFStudioApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	bFirstStart=TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGui_DevStudioApp object

CFFStudioApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioApp initialization

HMODULE m_hSciDLL = NULL;

BOOL CFFStudioApp::InitInstance()
{
	m_hSciDLL = LoadLibrary(_T("SciLexer.dll"));
	if (m_hSciDLL == NULL)
	{ 
		AfxMessageBox(_T("SciLexer DLL is not installed."));
		return FALSE;
	}

	AfxEnableControlContainer();

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pDocTemplateSci = new CMultiDocTemplate(
		IDR_GUI_DETYPE,
		RUNTIME_CLASS(CFFStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFFStudioView));
	AddDocTemplate(m_pDocTemplateSci);

	m_pDocTemplateSplash = new CMultiDocTemplate(
		IDR_GUI_DETYPE,
		RUNTIME_CLASS(CMfcieDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CMfcieView));
	AddDocTemplate(m_pDocTemplateSplash);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CFFStudioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioApp message handlers


void CFFStudioApp::OnFileNew() 
{
	// TODO: Add your command handler code here
	if (bFirstStart==TRUE)
	{
		bFirstStart=FALSE;
		CDocument* pDoc = m_pDocTemplateSplash->OpenDocumentFile(NULL);
	    pDoc->SetTitle(_T("Start Page"));
	}
	else
	{
		CWinApp::OnFileNew();
	}
}

int CFFStudioApp::ExitInstance()
{
	if (m_hSciDLL)
	{
		FreeLibrary(m_hSciDLL);
	}

	return CWinApp::ExitInstance();
}

#if 0
void CFFApp::OnSettingDlg()
{
	setting dlg;
	dlg.DoModal();
}

void CGui_DevStudioApp::OnFileOpen()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL);

	if(fileDlg.DoModal() == IDOK){
		m_pDocScintilla->OpenDocumentFile(fileDlg.GetPathName());
	}
}

void CGui_DevStudioApp::OnProjectNewproject()
{
	NewProjectDlg dlg;
	CWinThread* pThread;
	if(dlg.DoModal() == IDOK){
		 pThread = AfxBeginThread(MyThreadProc, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
        pThread->ResumeThread();        //start the thread
	}
}

UINT MyThreadProc(LPVOID pParam)
{
	return 0;
}

void ScanDir(CString dir)
{
       CFileFind fileFinder;
       CString filePath = dir + _T("//*.*");
      
       BOOL bFinished = fileFinder.FindFile(filePath);
       while(bFinished) 
       {
              bFinished = fileFinder.FindNextFile();
              if(fileFinder.IsDirectory() && !fileFinder.IsDots())  
              {
                     ScanDir(fileFinder.GetFilePath());
              }
              else
              {

                     CString fileName = fileFinder.GetFileName();
                     int dotPos=fileName.ReverseFind('.');
                     CString fileExt=fileName.Right(fileName.GetLength()-dotPos);
                     if(fileExt == _T(".txt")) 
                     {
                            CString category = fileName.Left(fileName.GetLength()-8); 
                            /*CString result = bt->StartTest(fileFinder.GetFilePath()); 
                           
                            int n = m_ListCtrl.GetItemCount();
                            m_ListCtrl.InsertItem(n, fileName,0);
                            m_ListCtrl.SetItemText( n,1, category) ;
                            m_ListCtrl.SetItemText(n,2, result) ;
                            if(category == result)
                                   m_ListCtrl.SetItemText(n,3, _T("Correct!")) ;  
                            else
                                   m_ListCtrl.SetItemText(n,3, _T("Wrong!")) ; 
							*/
                     }
              }
       }
 
       fileFinder.Close();
}
#endif

void CFFStudioApp::OnProjectAddproject()
{
	AddProjectDlg dlg;
	if(dlg.DoModal() == IDOK){
		((CMainFrame  *)AfxGetMainWnd()) ->m_GuiSolEx.m_TreeSolExplorer.DisplayProject(dlg.m_Name,
			dlg.m_Path);
	
#if 0
		sqlite3 *db=NULL;
		char *pErrMsg = NULL;
		char sql[512];
		int ret = 0;

		if(sqlite3_open("config.db", &db) != SQLITE_OK){
			sqlite3_close(db);
			return;
		}

		/*strcpy(sql, "CREATE TABLE project(ID INTEGER PRIMARY KEY AUTOINCREMENT,name VARCHAR(64),path VARCHAR(512));");
		ret = sqlite3_exec( db , sql , 0 , 0 , &pErrMsg );

		if ( ret != SQLITE_OK )
		{
			sqlite3_free(pErrMsg);
			return;
		}*/

		char *name = NULL, *path = NULL;
		CStringToChar(dlg.m_Name, name);
		CStringToChar(dlg.m_Path, path);
		//≤Â»Î ˝æ›
		sprintf(sql, "INSERT INTO project VALUES(NULL,'%s','%s');",name, path) ;
        ret = sqlite3_exec( db ,  sql , 0 , 0 , &pErrMsg );

		if(name != NULL){
			delete[] name;
		}

		if(path != NULL){
			delete[] path;
		}

		if ( ret != SQLITE_OK ){
			sqlite3_free(pErrMsg);
			return;
		}

		sqlite3_close(db);
#endif
	}
}


void CFFStudioApp::OnProjectOpenproject()
{
	OpenProjectDlg dlg;
	dlg.DoModal();
}
