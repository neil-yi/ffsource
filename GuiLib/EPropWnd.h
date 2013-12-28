// EPropWnd.h: interface for the EPropWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPROPWND_H__5E144A53_9017_412C_BB8C_14BB2407CA47__INCLUDED_)
#define AFX_EPROPWND_H__5E144A53_9017_412C_BB8C_14BB2407CA47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//static const UINT UWM_PROPERTY_LOOSING_FOCUS = ::RegisterWindowMessage(_T("EPROPERTYLIB_UWM_PROPERTY_LOOSING_FOCUS"));


class EProperty;
class IPropertyChangeListener;
#include "ECommentPane.h"

//PropWnd
//|
//+-- PropListCtrl
//|
//+-- GridCtrl


class GUILIBDLLEXPORT EPropWnd : public CWnd
{

	DECLARE_MESSAGE_MAP();
	DECLARE_DYNCREATE(EPropWnd);

	static CString s_FitStringToWidth( CString s , int nRequiredWidth , CDC* pDC );

protected:

	IPropertyChangeListener* m_pChangeListener;
	COLORREF m_cLigh;
	COLORREF m_cPane;
	COLORREF m_cDark;
	COLORREF m_cOuts;
	CPen m_PenLigh;
	CPen m_PenDark;
	CFont m_Font;
	int m_nFontHeight;
	int m_nEdgeWidth;

protected:

	EProperty* m_pActiveProperty;

	EProperty* GetActiveProperty()
	{
		return m_pActiveProperty;
	}

	void SetActiveProperty( EProperty* pProperty );

public:

	CPoint m_vpo;

public:

	ECommentPane* m_pCommentPane;

	bool m_bApplyOnKillFocus;
	bool m_bCloseOnKillFocus;

protected:

	void NotifyPropertyChanged();

	virtual CSize GetScrollPageSize();

public:

	CPoint _CP2SP( CPoint cp );
	CRect _CP2SP( CRect cp );
	void SetScrollPage();
	bool SetActiveProperty( EProperty* pNewActiveProperty , bool bApply , bool bVetoClosure );
	void EnsureVisibility( EProperty* pProperty );
	void EnsureVisibility( const CRect& r );

public:

	EPropWnd();
	virtual ~EPropWnd();

	void DrawFrame( CDC* pDC , const CRect& r , bool bRaised , int nColor );
	void DrawNipple( CDC* pDC , int x , int y , bool bRaised );
	virtual void GetIDRange( int& nID_PROPERTY_RANGE_BEG , int& nID_PROPERTY_RANGE_END );

//	void PropertyChanged();
	
	void ApplyActiveProperty();
	void CloseActiveProperty();

	int GetFontHeight();


	COLORREF GetColorPaneKey();
	COLORREF GetColorPaneKeyActive();
	COLORREF GetColorPaneVal();
	COLORREF GetColorPaneValActive();

	CFont* GetFont();


	//{{AFX_MSG(EPropWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditKillFocus();
	//}}AFX_MSG
//	afx_msg LRESULT OnUwmEditKillFocus(WPARAM, LPARAM);

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

};

#endif // !defined(AFX_EPROPWND_H__5E144A53_9017_412C_BB8C_14BB2407CA47__INCLUDED_)
