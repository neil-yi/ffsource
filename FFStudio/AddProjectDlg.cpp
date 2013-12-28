// AddProjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FFStudio.h"
#include "AddProjectDlg.h"


// AddProjectDlg �Ի���

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


// AddProjectDlg ��Ϣ�������

void AddProjectDlg::OnBnClickedPath()
{
	LPITEMIDLIST rootLoation;
SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP, &rootLoation );
if ( rootLoation == NULL ) {
   // unkown error
   // return
}
// ���öԻ���
BROWSEINFO bi;
ZeroMemory( &bi, sizeof( bi ) );
bi.pidlRoot = rootLoation; // �ļ��жԻ���֮��Ŀ¼����ָ���Ļ���Ϊ�ҵĵ���
bi.lpszTitle = _T( "Project Path" ); // ���Բ�ָ��
// bi.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

// �򿪶Ի���, �е���DoModal
LPITEMIDLIST targetLocation = SHBrowseForFolder( &bi );
if ( targetLocation != NULL ) {
   TCHAR targetPath[ MAX_PATH ];
   SHGetPathFromIDList( targetLocation, targetPath );
   //MessageBox( targetPath );
   GetDlgItem(IDC_PRO_PATH)->SetWindowText(targetPath);
}
}
