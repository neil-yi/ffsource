// AddProjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FFStudio.h"
#include "AddProjectDlg.h"


// AddProjectDlg 对话框

IMPLEMENT_DYNAMIC(AddProjectDlg, CDialog)

AddProjectDlg::AddProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AddProjectDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Path(_T(""))
{

}

AddProjectDlg::~AddProjectDlg()
{
}

void AddProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRO_NAME, m_Name);
	DDX_Text(pDX, IDC_PRO_PATH, m_Path);
}


BEGIN_MESSAGE_MAP(AddProjectDlg, CDialog)
	ON_BN_CLICKED(IDC_PATH, &AddProjectDlg::OnBnClickedPath)
END_MESSAGE_MAP()


// AddProjectDlg 消息处理程序

void AddProjectDlg::OnBnClickedPath()
{
	LPITEMIDLIST rootLoation;
SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP, &rootLoation );
if ( rootLoation == NULL ) {
   // unkown error
   // return
}
// 配置对话框
BROWSEINFO bi;
ZeroMemory( &bi, sizeof( bi ) );
bi.pidlRoot = rootLoation; // 文件夹对话框之根目录，不指定的话则为我的电脑
bi.lpszTitle = _T( "Project Path" ); // 可以不指定
// bi.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

// 打开对话框, 有点像DoModal
LPITEMIDLIST targetLocation = SHBrowseForFolder( &bi );
if ( targetLocation != NULL ) {
   TCHAR targetPath[ MAX_PATH ];
   SHGetPathFromIDList( targetLocation, targetPath );
   //MessageBox( targetPath );
   GetDlgItem(IDC_PRO_PATH)->SetWindowText(targetPath);
}
}
