#pragma once
#include "afxwin.h"


// OpenProjectDlg �Ի���

class OpenProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(OpenProjectDlg)

public:
	OpenProjectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~OpenProjectDlg();

// �Ի�������
	enum { IDD = IDD_OPEN_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_ProList;
};
