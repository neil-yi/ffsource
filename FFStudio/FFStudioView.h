// Gui_DevStudioView.h : interface of the CGui_DevStudioView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUI_DEVSTUDIOVIEW_H__551AA264_1A61_43F4_994C_7E3395AD3AB5__INCLUDED_)
#define AFX_GUI_DEVSTUDIOVIEW_H__551AA264_1A61_43F4_994C_7E3395AD3AB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScintillaDocView.h"
#include "ffstudiodoc.h"

class CFFStudioView : public CScintillaView
{
protected: // create from serialization only
	CFFStudioView();
	DECLARE_DYNCREATE(CFFStudioView)

// Attributes
public:
	CFFStudioDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGui_DevStudioView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFFStudioView();
/*#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
*/
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGui_DevStudioView)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();

protected:
	void SetAStyle(int style, COLORREF fore, COLORREF back=RGB(0xff, 0xff, 0xff), int size=-1, const char *face=0);
    void DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back);
};

#ifndef _DEBUG  // debug version in Gui_DevStudioView.cpp
inline CFFStudioDoc* CFFStudioView::GetDocument()
   { return (CFFStudioDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUI_DEVSTUDIOVIEW_H__551AA264_1A61_43F4_994C_7E3395AD3AB5__INCLUDED_)
