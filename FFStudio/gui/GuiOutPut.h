#pragma once
#include <afxhtml.h> 
#include "guicontrolbar.h"
#include "GuiTabWnd.h"
#include "mfcieVw.h"
#include "../FFStudioView.h"

class CGuiOutPut :	public CGuiControlBar
{
public:
	CGuiTabWnd	m_TabSolExplorer;
	CFFStudioView* m_pDetailView;
	CMfcieView* m_pCommentEdit;
	CEdit		m_EditFind;
	CListCtrl   m_Search;

public:
	CGuiOutPut(void);
	virtual ~CGuiOutPut(void);
	virtual void StyleDispl(DWORD dwDsp=GUISTYLE_XP)
	{
		m_StyleDisplay=dwDsp;
		m_TabSolExplorer.StyleDispl(dwDsp);
		CGuiControlBar::StyleDispl(dwDsp);
		
	}
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
