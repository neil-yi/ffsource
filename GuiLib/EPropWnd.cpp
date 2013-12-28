// EPropWnd.cpp: implementation of the EPropWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EProperty.h"
#include "IPropertyChangeListener.h"
#include "ECommentPane.h"
#include "IPropertyHost.h"
#include "EPropWnd.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(EPropWnd, CWnd)

BEGIN_MESSAGE_MAP(EPropWnd, CWnd)
	//{{AFX_MSG_MAP(CPropListView)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_EN_KILLFOCUS( 600 , OnEditKillFocus )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CString EPropWnd::s_FitStringToWidth( CString s , int nRequiredWidth , CDC* pDC )
{
	while( true )
	{
		int nTextWidth = pDC->GetTextExtent(s).cx;

		if( nTextWidth <= nRequiredWidth )
		{
			break;
		}

		int n = s.GetLength();

		if( n>=4 )
		{
			s = s.Left( n-4 );	//chop away 4
			s += "...";						//then add "..."
		}
		else
		{
			s.Empty();

			if( n==1 )	//there was one character in string, and it was too big!
			{
				break;
			}

			for( int i=0 ; i<n-1 ; i++ )
			{
				s += ".";
			}
		}
	}

	return s;
}

void EPropWnd::OnEditKillFocus()
{
	TR( _T("EPropWnd::OnEditKillFocus()") , _T("the edit box is loosing focus") );

	if( m_bApplyOnKillFocus )
	{
		ApplyActiveProperty();
	}

	if( m_bCloseOnKillFocus )
	{
		CloseActiveProperty();
	}

	m_bApplyOnKillFocus = true;
	m_bCloseOnKillFocus = true;
}


EPropWnd::EPropWnd()
{
	m_cPane = ::GetSysColor( COLOR_3DFACE    );
	m_cLigh = ::GetSysColor( COLOR_3DHILIGHT );
	m_cDark = ::GetSysColor( COLOR_3DSHADOW  );
	m_cOuts = m_cPane - 0x505050;

	m_PenLigh.CreatePen( PS_SOLID , 0 , m_cLigh );
	m_PenDark.CreatePen( PS_SOLID , 0 , m_cDark );

	m_pChangeListener = NULL;

	SetActiveProperty(NULL);

	m_nFontHeight = 13;

	m_Font.DeleteObject();
	m_Font.CreateFont( m_nFontHeight , 0, 0, 0, FW_MEDIUM, FALSE,	FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH , _T("tahoma") );

	m_nEdgeWidth  = 1;

	m_bApplyOnKillFocus = true;
	m_bCloseOnKillFocus = true;

	m_pCommentPane = NULL;
}

EPropWnd::~EPropWnd()
{

}


void EPropWnd::GetIDRange( int& nID_PROPERTY_RANGE_BEG , int& nID_PROPERTY_RANGE_END )
{
	nID_PROPERTY_RANGE_BEG = 600;
	nID_PROPERTY_RANGE_END = 700;
}

//void EPropWnd::PropertyChanged()
//{
//	NotifyPropertyChanged();
//}

void EPropWnd::CloseActiveProperty()
{
	TR( _T("EPropWnd::CloseActiveProperty()"),(GetActiveProperty()==NULL)?(_T("NULL")):(GetActiveProperty()->GetName()));

	if( GetActiveProperty()!=NULL )
	{
		m_bApplyOnKillFocus = false;
		m_bCloseOnKillFocus = false;
		GetActiveProperty()->OnPropClose();	//causes killfocus on properties with cedit control!
		SetActiveProperty(NULL);
	}
}

void EPropWnd::ApplyActiveProperty()
{
	TR( _T("EPropWnd::ApplyActiveProperty()"),(GetActiveProperty()==NULL)?( _T("NULL") ):(GetActiveProperty()->GetName()));

	if( GetActiveProperty() != NULL )
	{
		if( GetActiveProperty()->IsEnabled() )
		{
			bool bChanged = GetActiveProperty()->OnPropApply();

			if( bChanged )
			{
				if( GetActiveProperty()->m_pParentHost!=NULL )
				{
					GetActiveProperty()->m_pParentHost->PropertyChanging( GetActiveProperty()->m_pProperty , NULL );
				}

				NotifyPropertyChanged();
			}

			Invalidate();
		}
	}
}


int EPropWnd::GetFontHeight()
{
	return m_nFontHeight;
}

COLORREF EPropWnd::GetColorPaneKey()
{
	return m_cPane;
}
COLORREF EPropWnd::GetColorPaneVal()
{
	return RGB(255,255,255);
}
COLORREF EPropWnd::GetColorPaneKeyActive()
{
	return m_cPane - 0x202020;
}
COLORREF EPropWnd::GetColorPaneValActive()
{
	return RGB(255,255,255);	//m_cPane - 0x202020;
}

CFont* EPropWnd::GetFont()
{
	return &m_Font;
}


void EPropWnd::NotifyPropertyChanged()
{
	if( m_pChangeListener != NULL )
	{
		// maybe change propertylist as property changes, combo-choises may add/remove properties from list??

		//RefreshHostList();	//this was enabled again to provide change in gui in case of combobox change. It was once disabled, but as for now i cant remember why..?
								//disabled again, because tab was set out of order..

		// SetPropertyHost(m_pPropertyHost);
		m_pChangeListener->PropertyChanged();
	}

//	Invalidate();
}

void EPropWnd::OnSize(UINT nType, int cx, int cy) 
{
	cy -= 20;

	CWnd::OnSize(nType, cx, cy);

	SetScrollPage();
}


void EPropWnd::SetScrollPage()
{
	TR( _T("EPropWnd::SetScrollPage()"), _T(""));

	CSize PageSize = GetScrollPageSize();

	if( PageSize.cx==0 && PageSize.cy==0 )
	{
		SetScrollPos ( SB_HORZ , 0     );
		SetScrollPos ( SB_VERT , 0     );
		ShowScrollBar( SB_BOTH , FALSE );
	}
	else
	{
		CRect wr;
		GetWindowRect(&wr);
		CSize ClientSize = wr.Size();

		bool bEnableH = false;
		bool bEnableV = false;

		if( PageSize.cx > ClientSize.cx )
		{
			ClientSize.cy -= GetSystemMetrics(SM_CYHSCROLL);
			bEnableH = true;
		}

		if( PageSize.cy > ClientSize.cy )
		{
			ClientSize.cx -= GetSystemMetrics(SM_CXVSCROLL);
			bEnableV = true;

			if( bEnableH == false )
			{
				if( PageSize.cx > ClientSize.cx )
				{
					ClientSize.cy -= GetSystemMetrics(SM_CYHSCROLL);
					bEnableH = true;
				}
			}
		}

		//
		//
		//

		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask  = SIF_PAGE | SIF_POS | SIF_RANGE;

		//
		// horizontal scroller
		//

		si.nPage  = bEnableH ? ClientSize.cx         : 0;
		si.nMin   = bEnableH ? 0                     : 0;
		si.nMax   = bEnableH ? PageSize.cx-1         : 0;
		si.nPos   = bEnableH ? GetScrollPos(SB_HORZ) : 0;
		SetScrollInfo( SB_HORZ , &si );
		ShowScrollBar( SB_HORZ , bEnableH );

		//
		// vertical scroller
		//

		si.nPage  = bEnableV ? ClientSize.cy         : 0;
		si.nMin   = bEnableV ? 0                     : 0;
		si.nMax   = bEnableV ? PageSize.cy-1         : 0;
		si.nPos   = bEnableV ? GetScrollPos(SB_VERT) : 0;
		SetScrollInfo( SB_VERT , &si );
		ShowScrollBar( SB_VERT , bEnableV );
	}
}


void EPropWnd::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	int nNewPos = -1;
	int sp = GetScrollPos(SB_HORZ);

	if( nSBCode==SB_THUMBPOSITION || nSBCode==SB_THUMBTRACK )
	{
		nNewPos = nPos;
	}
	else if( nSBCode==SB_LINELEFT )
	{
		sp -= 5;
		nNewPos = sp;
	}
	else if( nSBCode==SB_LINERIGHT )
	{
		sp+=5;
		nNewPos = sp;
	}
	else if( nSBCode==SB_PAGELEFT )
	{
		sp-=20;
		nNewPos = sp;
	}
	else if( nSBCode==SB_PAGERIGHT )
	{
		sp+=20;
		nNewPos = sp;
	}
	
	if( nNewPos >= 0 )
	{
		SetScrollPos( SB_HORZ , nNewPos );
		Invalidate();
	}
}

//Thanks to Damian Trebilco for helping out on the mouse scrolling
BOOL EPropWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UINT nCode = 0;

	if( zDelta < 0 )
	{
		nCode = SB_LINEDOWN;
	}
	else
	{
		nCode = SB_LINEUP;
	}

	OnVScroll( nCode , 0 , NULL );

	return CWnd::OnMouseWheel( nFlags , zDelta , pt );
}


//DWORD g_nLastScrollTime = 0;

void EPropWnd::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
//	     if( nSBCode == SB_THUMBPOSITION ) TRACE("SB_THUMBPOSITION pos=%d\n" , nPos );
//	else if( nSBCode == SB_THUMBTRACK    ) TRACE("SB_THUMBTRACK    pos=%d\n" , nPos );
//	else if( nSBCode == SB_LINEUP        ) TRACE("SB_LINEUP        pos=%d\n" , nPos );
//	else if( nSBCode == SB_LINEDOWN      ) TRACE("SB_LINEDOWN      pos=%d\n" , nPos );
//	else if( nSBCode == SB_PAGEUP        ) TRACE("SB_PAGEUP        pos=%d\n" , nPos );
//	else if( nSBCode == SB_PAGEDOWN      ) TRACE("SB_PAGEDOWN      pos=%d\n" , nPos );
//	else if( nSBCode == SB_ENDSCROLL     ) TRACE("SB_ENDSCROLL     pos=%d\n" , nPos );
//	else                                   TRACE("??               pos=%d\n" , nPos );

//	TR("EPropWnd::OnVScroll()" , "" );

	bool bSetPos = true;
	int nOldPos = GetScrollPos(SB_VERT);
	int nNewPos = 0x40000000;

	if( nSBCode != SB_ENDSCROLL )
	{
//		DWORD nNow = GetTickCount();
//		DWORD nDelta = nNow - g_nLastScrollTime;
//		TRACE("scroll delta =                       %d ms\n" , nDelta );
//		g_nLastScrollTime = nNow;

//		if( nDelta > 5 )
//		{
			if( nSBCode==SB_THUMBPOSITION || nSBCode==SB_THUMBTRACK )
			{
				nNewPos = nPos;
			}
			else
			{
				nNewPos = nOldPos;

				switch( nSBCode )
				{
				case SB_LINEUP:
					nNewPos -= 8;
					break;
				case SB_LINEDOWN:
					nNewPos += 8;
					break;
				case SB_PAGEUP:
					nNewPos -= 24;
					break;
				case SB_PAGEDOWN:
					nNewPos += 24;
					break;
				}
			}

//			TRACE("oldpos=%d newpos=%d\n" , nOldPos , nNewPos );
			SetScrollPos( SB_VERT , nNewPos );
			Invalidate();

//		}
	}
}

void EPropWnd::DrawFrame( CDC* pDC , const CRect& r , bool bRaised , int nColor )
{
	CRect rect;

	if( bRaised )
	{
		pDC->SelectObject( &m_PenLigh );
	}
	else
	{
		pDC->SelectObject( &m_PenDark );
	}

	pDC->MoveTo( r.left  , r.bottom-1 );
	pDC->LineTo( r.left  , r.top );
	pDC->LineTo( r.right-1 , r.top );

	if( bRaised )
	{
		pDC->SelectObject( &m_PenDark );
	}
	else
	{
		pDC->SelectObject( &m_PenLigh );
	}

	pDC->LineTo( r.right-1 , r.bottom-1 );
	pDC->LineTo( r.left , r.bottom-1 );

	if( nColor != -1 )
	{
		rect = r;
		rect.DeflateRect(m_nEdgeWidth,m_nEdgeWidth);
		pDC->FillSolidRect( &rect , nColor );
	}
}

void EPropWnd::DrawNipple( CDC* pDC , int x , int y , bool bRaised )
{
	if( bRaised )
	{
		pDC->FillSolidRect( x+1 , y+1 , 2 , 2 , m_cDark );
		pDC->FillSolidRect( x   , y   , 2 , 2 , m_cLigh );
	}
	else
	{
		pDC->FillSolidRect( x+1 , y+1 , 2 , 2 , m_cLigh );
		pDC->FillSolidRect( x   , y   , 2 , 2 , m_cDark );
	}
}

bool EPropWnd::SetActiveProperty( EProperty* pNewActiveProperty , bool bApply , bool bVetoClosure )
{
	TR( _T("EPropWnd::SetActiveProperty()"),(pNewActiveProperty==NULL)?( _T("NULL")):(pNewActiveProperty->GetName()));

	bool bSuccess = true;

	if( pNewActiveProperty != GetActiveProperty() )
	{
		//
		// apply old
		//

		if( GetActiveProperty() != NULL )
		{
			if( bApply )
			{
				if( bVetoClosure )
				{
					bSuccess = GetActiveProperty()->CanApply();	//dont apply and dont close
				}

				if( bSuccess )
				{
					ApplyActiveProperty();
				}
			}
		}

		//
		// close old
		//

		if( bSuccess )
		{
			CloseActiveProperty();
		}
		else
		{
			MessageBeep( MB_ICONASTERISK );
		}

		//
		// set new active property
		//

		if( bSuccess )
		{
			SetActiveProperty(pNewActiveProperty);

			if( GetActiveProperty() != NULL )
			{
				GetActiveProperty()->OnPropActivate(this);
				EnsureVisibility( GetActiveProperty() );
			}

			Invalidate();								//only update this view (selection changed)

			if( m_pCommentPane != NULL )
			{
				m_pCommentPane->Invalidate();
			}
		}
	}

	return bSuccess;
}

CPoint EPropWnd::_CP2SP( CPoint cp )
{
	return cp - m_vpo;
}

CRect EPropWnd::_CP2SP( CRect cp )
{
	CRect r = cp;
	r.OffsetRect( -m_vpo );
	return r;
}

CSize EPropWnd::GetScrollPageSize()
{
	return CSize(100,100);
}

BOOL EPropWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int nBeg;
	int nEnd;
	GetIDRange(nBeg,nEnd);

//	TRACE("PropView wParam=%d lParam=%d!\n" , wParam , lParam );

	int nCommandID           = LOWORD(wParam);	//command of menu, control or accelerator
	int nNotificationMessage = HIWORD(wParam);	//notification message if this is from a control

	bool bMessageIsFromAccelerator = (HIWORD(wParam)==1);
	bool bMessageIsFromMenu        = (HIWORD(wParam)==0);
	bool bMessageFromControl       = (bMessageIsFromAccelerator==false) && (bMessageIsFromMenu==false);

	if( bMessageFromControl )
	{
		int nControlThatSendTheMessage = lParam;
	}

	if( nNotificationMessage==0 )
	{
		if( nBeg <= nCommandID && nCommandID <= nEnd )
		{
			if( GetActiveProperty() != NULL )
			{
				bool bChanged = GetActiveProperty()->OnCommand(this,wParam,lParam);

				if( bChanged )
				{
					NotifyPropertyChanged();
				}

				Invalidate();

			}

			return TRUE;
		}
	}

	return CWnd::OnCommand(wParam, lParam);
}

void EPropWnd::EnsureVisibility( EProperty* pProperty )
{
	if( pProperty!=NULL )
	{
		CRect r = pProperty->m_Rect;
		EnsureVisibility(r);
	}
}

void EPropWnd::EnsureVisibility( const CRect& r )
{
	CRect cr;
	GetClientRect(&cr);

	CPoint tl = _CP2SP( cr.TopLeft()     );
	CPoint br = _CP2SP( cr.BottomRight() );
	cr.SetRect( tl , br );

	int nCurrentScrollPos = GetScrollPos( SB_VERT );

	int nDeltay = 0;

	int nsp = -1;

	if( r.bottom > cr.bottom )
	{
		nsp = r.bottom - cr.Height();
		ASSERT(nsp>=0);

		cr.OffsetRect( 0 , nsp );
	}

	if( r.top < cr.top )							//hvis toppen kom over øverste..
	{
		nsp = r.top;
		ASSERT(nsp>=0);
	}

	if( nsp>=0 )
	{
		SetScrollPos( SB_VERT , nsp );
	}
}

void EPropWnd::SetActiveProperty( EProperty* pProperty )
{
	TR( _T("EPropWnd::SetActiveProperty()") , (pProperty==NULL) ? _T("NULL") : pProperty->GetName() );
	m_pActiveProperty = pProperty;
}
