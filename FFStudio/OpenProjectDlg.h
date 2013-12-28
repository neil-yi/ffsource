#pragma once
#include "afxwin.h"


// OpenProjectDlg 对话框

class OpenProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(OpenProjectDlg)

public:
	OpenProjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~OpenProjectDlg();

// 对话框数据
	enum { IDD = IDD_OPEN_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_ProList;
};
