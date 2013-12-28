#pragma once
#include <afxhtml.h> 
#include "guicontrolbar.h"
#include "GuiTabWnd.h"
#include "mfcieVw.h"
class CGuiOutPut :	public CGuiControlBar
{
protected:
	CGuiTabWnd	m_TabSolExplorer;
	//CListCtrl   m_ListTask;
	CMfcieView* m_pListTask;
	//CHtmlCtrl m_ListTask;
	CRichEditCtrl		m_EditOutput;
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
