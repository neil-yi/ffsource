// ItemOverlayEdit.cpp : implementation file
//

#include "stdafx.h"
#include "EPropWnd.h"
#include "EPropWndEdit.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EPropWndEdit

EPropWndEdit::EPropWndEdit()
{
}

EPropWndEdit::~EPropWndEdit()
{
}


BEGIN_MESSAGE_MAP(EPropWndEdit, CEdit)
	//{{AFX_MSG_MAP(EPropWndEdit)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EPropWndEdit message handlers

void EPropWndEdit::OnKillFocus( CWnd* pWnd )
{
	TR( _T("EPropWndEdit::OnKillFocus()") , _T("dont know who gets focus") );
	
	bool bNextFocusIsButton = false;
	
	if( pWnd != NULL )
	{
		int nID = pWnd->GetDlgCtrlID();
		bNextFocusIsButton = (nID==601);
	}

	EPropWnd* pPropWnd = (EPropWnd*)GetParent();

	if( bNextFocusIsButton )
	{
		pPropWnd->m_bCloseOnKillFocus = false;
		pPropWnd->m_bApplyOnKillFocus = false;
	}

//	GetParent()->PostMessage( UWM_PROPERTY_LOOSING_FOCUS , nID==601 );

	CEdit::OnKillFocus(pWnd);
}
