// DirTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "FFStudio.h"
#include "DirTreeCtrl.h"

#include "MainFrm.h"
#include "ffstudioview.h"

// CDirTreeCtrl

IMPLEMENT_DYNAMIC(CDirTreeCtrl, CTreeCtrl)

CDirTreeCtrl::CDirTreeCtrl()
{

}

CDirTreeCtrl::~CDirTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CDirTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CDirTreeCtrl::OnTvnItemexpanding)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CDirTreeCtrl::OnNMDblclk)
END_MESSAGE_MAP()


BOOL CDirTreeCtrl::DisplayTree(LPCTSTR strRoot)
{
	SetItemHeight(20);
	SetTextColor(RGB(0X0,0X0,0XFF));
	DeleteAllItems();
	SetDirTreeStyle();
	//if ( !AttachImgList() )
	//	return FALSE;
	
	DisplayDrives();

	return TRUE;
}

void CDirTreeCtrl::SetDirTreeStyle()
{
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE );
	dwStyle |= TVS_HASBUTTONS |
		TVS_HASLINES | TVS_LINESATROOT |
		/*TVS_CHECKBOXES |*/ 
		WS_BORDER | WS_TABSTOP;
		m_treeStyle = dwStyle;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle );
} 

BOOL CDirTreeCtrl::AttachImgList()
{
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;
	if ( GetImageList( TVSIL_NORMAL ) ) m_imgList.Detach();
	
	hImgList = (HIMAGELIST)SHGetFileInfoW(     _T("C:\\"),
		0,         &shFinfo,         sizeof( shFinfo ),
		SHGFI_SYSICONINDEX |  SHGFI_SMALLICON );
	if ( !hImgList )     {
		m_strError = _T("无法得到系统图标文件!");
		return FALSE;
	}
	m_imgList.m_hImageList = hImgList;
	SetImageList( &m_imgList, TVSIL_NORMAL );
	return TRUE;  
}

BOOL CDirTreeCtrl::DisplayDrives()
{
	DeleteAllItems();
	TCHAR  szDrives[260];
	TCHAR* pDrive=NULL;
	if ( !GetLogicalDriveStrings( sizeof(szDrives), szDrives ) )
		{          m_strError =_T("驱动信息获取失败！");
	return FALSE;
	} 
	pDrive = szDrives;    //szDrives 中的字符串格式：_T("C:\\0D:\\0D:\\0E:\\0")
	m_hDirTreeRoot = InsertItem(_T("project"));

	int len;
	while( *pDrive!=0 )
	{
		len = _tcslen(pDrive);
		pDrive[len-1] = _T('\0');
		HTREEITEM hParent = AddItem( m_hDirTreeRoot, pDrive );
		if ( FindSubDir( pDrive ))  AddSubDirAsItem(hParent);
		
		pDrive += len + 1;
	}

	Expand( m_hDirTreeRoot, TVE_EXPAND );
	return TRUE;
}

BOOL CDirTreeCtrl::DisplayProject(LPCTSTR proName, LPCTSTR proPath)
{
	HTREEITEM root;
	SetItemHeight(20);
	SetTextColor(RGB(0X0,0X0,0XFF));
	DeleteAllItems();
	SetDirTreeStyle();
	if ( !AttachImgList() )
		return FALSE;

	m_RootName = proName;
	m_RootPath = proPath;
	
	root = InsertItem(proName);
	BuildTree(root, proPath);
	
	Expand( root, TVE_EXPAND );
	return TRUE;
}

BOOL CDirTreeCtrl::BuildTree(HTREEITEM hParent,LPCTSTR strPath)
{      
	CFileFind find;
	HTREEITEM parent;
	CString   strTemp = strPath;
	BOOL      bFind;
	if ( strTemp.Right(1) == _T('\\') ){
		strTemp += _T("*.*");
	}else{
		strTemp += _T("\\*.*");
	}
	bFind = find.FindFile( strTemp );

	while ( bFind ) {
		bFind = find.FindNextFile();

		if ( find.IsDirectory()){
			if(!find.IsDots()){
				parent = AddItem1(hParent, find.GetFilePath(), find.GetFileName());
				BuildTree(parent, find.GetFilePath());
			}
		}else{
			AddItem1(hParent, find.GetFilePath(), find.GetFileName());
		}
	}
	
	return TRUE; 
}

HTREEITEM CDirTreeCtrl::AddItem1(HTREEITEM hParent, LPCTSTR path, LPCTSTR name)
{
	SHFILEINFO shFinfo;
	int iIcon, iIconSel;
	if ( !SHGetFileInfo( path, 
		0,   &shFinfo,   sizeof( shFinfo ),
		SHGFI_ICON |SHGFI_SMALLICON ) )  { 
			m_strError = _T("系统图表获取失败!");
			return NULL;
	}
	iIcon = shFinfo.iIcon;   // we only need the index from the system image list
	
	DestroyIcon( shFinfo.hIcon );

	if ( !SHGetFileInfo( path,   0,   &shFinfo,
		sizeof( shFinfo ),
		SHGFI_ICON | SHGFI_OPENICON |   SHGFI_SMALLICON ) )   {
			m_strError =  _T("系统图表获取失败!");
			return NULL;
	}
	iIconSel = shFinfo.iIcon;    // we only need the index of the system image list
	DestroyIcon( shFinfo.hIcon );
	return InsertItem( name, iIcon, iIconSel, hParent ); 
}

CString CDirTreeCtrl::GetFullPath1(HTREEITEM hItem)
{
	CString path, temp;

	HTREEITEM hParent = GetParentItem(hItem);
	while ( hParent )
	{ 
		temp  = GetItemText( hParent );
		if(temp != m_RootName){
			temp =  temp + _T("\\");
			path = temp + path;
		}
		hParent = GetParentItem(hParent);
	}
	path = _T("\\") + path;
	path = m_RootPath + path;
	return path;
}

CString CDirTreeCtrl::GetFullPath(HTREEITEM hItem)
{
	CString strReturn;
	CString strTemp;
	HTREEITEM hParent = hItem;
	strReturn = "";
	while ( hParent )
	{ 
		strTemp  = GetItemText( hParent );
		if(strTemp != _T("project"))   {
			if ( strTemp.Right(1) !=  _T("\\"))
				strTemp += _T("\\");
			strReturn = strTemp + strReturn;
		} 
		hParent = GetParentItem( hParent );
	}
	return strReturn;
}

BOOL CDirTreeCtrl::AddSubDirAsItem(HTREEITEM hParent)
{
	CString strPath,strFileName;
	HTREEITEM hChild;
	//---------------------去除该父项下所有的子项------------   // 因为有dummy项，并且有时子目录再次打开，或子目录会刷新等，因此必须去除。
	while ( ItemHasChildren(hParent))  {
		hChild = GetChildItem(hParent);
		DeleteItem( hChild );
	}  
	//-----------------------装入该父项下所有子项-------------- 
	strPath = GetFullPath(hParent);  // 从本节点开始到根的路径
	CString strSearchCmd = strPath;
	if( strSearchCmd.Right( 1 ) != _T( "\\" ))
		strSearchCmd += _T( "\\" );

	strSearchCmd += _T( "*.*" );
	CFileFind find;
	BOOL bContinue = find.FindFile( strSearchCmd );
	while ( bContinue )  {
		bContinue = find.FindNextFile();
		strFileName = find.GetFileName(); 

		if ( !find.IsHidden() && ! find.IsDots() && find.IsDirectory() )
		{ 
			hChild = AddItem( hParent, strFileName );
			if ( FindSubDir( GetFullPath(hChild) ))
				AddSubDirAsItem1(hChild); 
		}

		if ( !find.IsHidden() && ! find.IsDots() && !find.IsDirectory() )
		{
			InsertItem( strFileName, 0, 0, hParent );
		}
	}

	return TRUE; 
}

BOOL CDirTreeCtrl::AddSubDirAsItem1(HTREEITEM hParent)
{
	CString strPath,strFileName;
	HTREEITEM hChild;
	//---------------------去除该父项下所有的子项------------   // 因为有dummy项，并且有时子目录再次打开，或子目录会刷新等，因此必须去除。
	while ( ItemHasChildren(hParent))  {
		hChild = GetChildItem(hParent);
		DeleteItem( hChild );
	}    //-----------------------装入该父项下所有子项--------------  
	strPath = GetFullPath(hParent);

	CString strSearchCmd = strPath;
	if( strSearchCmd.Right( 1 ) != _T( "\\" ))
		strSearchCmd += _T( "\\" );

	strSearchCmd += _T( "*.*" );
	CFileFind find;
	BOOL bContinue = find.FindFile( strSearchCmd );
	while ( bContinue )  {
		bContinue = find.FindNextFile();
		strFileName = find.GetFileName();

		 if(!find.IsHidden() && ! find.IsDots() && find.IsDirectory() )
		 {
			 hChild = AddItem( hParent, strFileName );
		 }

		if ( !find.IsHidden() && ! find.IsDots() && !find.IsDirectory() )   {
			InsertItem( strFileName, 0, 0, hParent );
		}
    }

    return TRUE; 
}

HTREEITEM CDirTreeCtrl::AddItem(HTREEITEM hParent, LPCTSTR strName)
{  // 获取路径
	CString strPath = GetFullPath(hParent);
	CString strTemp = strPath + CString(strName);
	if ( strTemp.Right(1) !=  _T("\\") )
		strTemp += _T("\\");
	
	SHFILEINFO shFinfo;
	int iIcon, iIconSel;
	if ( !SHGetFileInfo( strTemp, 
		0,   &shFinfo,   sizeof( shFinfo ),
		SHGFI_ICON |SHGFI_SMALLICON ) )  { 
			m_strError = _T("系统图表获取失败!");
			return NULL;
	}
	iIcon = shFinfo.iIcon;   // we only need the index from the system image list
	
	DestroyIcon( shFinfo.hIcon );

	if ( !SHGetFileInfo( strTemp,   0,   &shFinfo,
		sizeof( shFinfo ),
		SHGFI_ICON | SHGFI_OPENICON |   SHGFI_SMALLICON ) )   {
			m_strError =  _T("系统图表获取失败!");
			return NULL;
	}
	iIconSel = shFinfo.iIcon;    // we only need the index of the system image list
	DestroyIcon( shFinfo.hIcon );
	return InsertItem( strName, iIcon, iIconSel, hParent ); 
}

BOOL CDirTreeCtrl::FindSubDir(LPCTSTR strPath)
{      
	CFileFind find;
	CString   strTemp = strPath;
	BOOL      bFind;
	if ( strTemp.Right(1) == _T('\\') ){
		strTemp += _T("*.*");
	}else{
		strTemp += _T("\\*.*");
	}
	bFind = find.FindFile( strTemp );

	while ( bFind )     {
		bFind = find.FindNextFile();
		if ( find.IsDirectory() && !find.IsDots() )         {
			return TRUE;
		}

		if ( !find.IsDirectory()/* && m_bShowFiles*/ )
			return TRUE;
	}
	  return FALSE; 
}


void CDirTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
#if 0	
	TV_ITEM tvi= pNMTreeView->itemNew;
	HTREEITEM hItem = tvi.hItem;
	
	if(NULL == hItem )
		return;
	if (_T("project")==GetItemText(hItem))
		return;
	AddSubDirAsItem(hItem);
#endif
	*pResult = 0;
}

void CDirTreeCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM m_hTreeItem = GetSelectedItem();

	CString strPath = GetFullPath1(m_hTreeItem);
	strPath += _T("\\");
	strPath += GetItemText(m_hTreeItem);

	((CFFStudioApp*)AfxGetApp())->m_pDocTemplateSci->OpenDocumentFile(strPath);

	*pResult = 0;
}
