// OpenProjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FFStudio.h"
#include "OpenProjectDlg.h"
#include "sqlite3.h"

// OpenProjectDlg 对话框

IMPLEMENT_DYNAMIC(OpenProjectDlg, CDialog)

OpenProjectDlg::OpenProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OpenProjectDlg::IDD, pParent)
{

}

OpenProjectDlg::~OpenProjectDlg()
{
}

void OpenProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRO_LIST, m_ProList);
}


BEGIN_MESSAGE_MAP(OpenProjectDlg, CDialog)
END_MESSAGE_MAP()


// OpenProjectDlg 消息处理程序

BOOL OpenProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	sqlite3 *db=NULL;
	if(sqlite3_open("config.db", &db) != SQLITE_OK){
		sqlite3_close(db);
		return FALSE;
    }

	int nrow = 0, ncolumn = 0, i=0, ret = 0;
	char **azResult;
    char *sql = "select name from project";
	char *pErrMsg;

    ret = sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &pErrMsg );

	if ( ret != SQLITE_OK )
    {
        sqlite3_free(pErrMsg);
		return FALSE;
    }

	CString str;
    for( i=0 ; i< nrow ; i++ )
    {
		str.Format(L"%s",azResult[i]);
		//m_ProList.AddString();
    }

	return TRUE;  // return TRUE unless you set the focus to a control
}
