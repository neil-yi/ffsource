#pragma once

class CDirTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CDirTreeCtrl)

public:
	CDirTreeCtrl();
	virtual ~CDirTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL DisplayTree(LPCTSTR strRoot);
	void SetDirTreeStyle();
	BOOL DisplayProject(LPCTSTR proName, LPCTSTR proPath);
	BOOL BuildTree(HTREEITEM hParent,LPCTSTR strPath);
	HTREEITEM AddItem1(HTREEITEM hParent, LPCTSTR path, LPCTSTR name);
	CString GetFullPath1(HTREEITEM hItem);

	CString m_RootName;
	CString m_RootPath;

private:
	BOOL AddSubDirAsItem(HTREEITEM hParent);
   BOOL AddSubDirAsItem1(HTREEITEM hParent);
   BOOL FindSubDir(LPCTSTR strPath);
   CString GetFullPath(HTREEITEM hItem);
   BOOL DisplayDrives();
   BOOL AttachImgList();
   HTREEITEM AddItem( HTREEITEM hParent, LPCTSTR strName );
   CString        m_strError;
   CImageList     m_imgList;
   HTREEITEM m_hDirTreeRoot;
   DWORD m_treeStyle;
public:
	afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


