#pragma once


// AddProjectDlg �Ի���

class AddProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(AddProjectDlg)

public:
	AddProjectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AddProjectDlg();

// �Ի�������
	enum { IDD = IDD_ADD_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPath();
	CString m_Name;
	CString m_Path;
};
