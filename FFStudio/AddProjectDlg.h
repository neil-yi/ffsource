#pragma once


// AddProjectDlg 对话框

class AddProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(AddProjectDlg)

public:
	AddProjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddProjectDlg();

// 对话框数据
	enum { IDD = IDD_ADD_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPath();
	CString m_Name;
	CString m_Path;
};
