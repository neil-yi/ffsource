
#ifndef EPROPLISTWND_H
#define EPROPLISTWND_H

class EProperty;
class IPropertyHost;

#include "EPropList.h"
#include "IPropertyChangeListener.h"
#include "EPropWnd.h"

class GUILIBDLLEXPORT EPropListWnd : public EPropWnd , public ECommentPane::ICommentHost
{

private:

	IPropertyHost* m_pPropertyHost;

	bool m_bVScrollEnabled;
	bool m_bHScrollEnabled;


	CSize m_ScrollPageSize;

protected:

	CRect m_SplitRectSP;

	int m_nKeyColumnWidth;	//col are widths inclusive frame edge
	bool m_bAutoSizeKeyColumn;

	int m_nSplitWidth;
	int m_nOptimalValColumnWidth;

	int m_nMouseDownKeyColumnWidth;
	int m_nMouseDownMousePositionSP;

	int m_nViewWidth;

	int m_nAnimFirstOpeningChild;
	int m_nAnimCurrentFrame;
	int m_nAnimChildrensHeight;
	bool m_bAnimating;

	//{{AFX_MSG(EPropListWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	afx_msg void OnTimer( UINT nTimer );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnKillFocus( CWnd* pNewWnd );
	//}}AFX_MSG

	virtual CSize GetScrollPageSize();

protected:
		
	DECLARE_MESSAGE_MAP();
	DECLARE_DYNCREATE(EPropListWnd);

protected:

	EPropList m_PropList;

	EProperty** m_pDrawnProperties;
	int m_nDrawnPropertiesSize;
	int m_nNumDrawnProperties;

public:
	
	//
	//
	//

	EPropListWnd();
	virtual ~EPropListWnd();

	//
	//
	//


//	#ifdef _DEBUG
//	virtual void AssertValid() const;
//	virtual void Dump(CDumpContext& dc) const;
//	#endif

	//{{AFX_VIRTUAL(EPropListWnd)
	public:
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	protected:
//	virtual void OnInitialUpdate(); // called first time after construct
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:

	IPropertyHost* GetPropertyHost(){return m_pPropertyHost;}
	void SetPropertyChangeListener( IPropertyChangeListener* pListener );
	void SetPropertyHost( IPropertyHost* pHost );
	void RefreshHostList();
	bool SetActiveTab(int i);
	EPropList* GetPropertyList();
	void EnableAutoSizeKeyColumn(bool bEnable=true)		{ m_bAutoSizeKeyColumn = bEnable; }
	bool ValidateItems();

protected:

	void _UpdateDrawnPropertiesList( CDC* pDC );
	void AutoSizeKeyColumn();
	void StepSelection( int nStep );
	void OpenNode( EProperty* pProp );
	void CloseNode( EProperty* pProp );

public:

	//
	// ICommentHost
	//
	virtual CString GetCurrentComment( CString& sObjectName );

};

#endif
