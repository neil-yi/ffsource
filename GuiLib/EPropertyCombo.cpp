/* Generated by Together */

#include "stdafx.h"
//#include "EPropListCtrl.h"
#include "EPropWnd.h"
#include "IPropertyHost.h"
#include "EPropertyCombo.h"

EPropertyCombo::EPropertyCombo( IPropertyHost* pHost , const CString& sName , int* pProperty ) : EIconTextButtonProperty( pHost , sName , pProperty , true )
{
}

CString EPropertyCombo::GetTextValue()
{
	int nCurrent = *(int*)m_pProperty;

	ASSERT(m_StringArray.GetSize()==m_Values.size());

	CString s = "N/A";

	for( unsigned int i=0 ; i<m_Values.size() ; i++ )
	{
		if( m_Values[i]==nCurrent )
		{
			s = m_StringArray[i];
			break;					//the string for this value was found, so break out
		}
	}

	return s;
}

bool EPropertyCombo::HasButton()
{
	return true;
}

EPropertyCombo* EPropertyCombo::AddString( const CString& s )
{
	return AddString( s , m_Values.size() );
}

EPropertyCombo* EPropertyCombo::AddString( const CString& s , int nValue )
{
	m_StringArray.Add(s);
	m_Values.push_back(nValue);
	return this;
}

EPropertyCombo* EPropertyCombo::AddSeparator()
{
	m_Separators.push_back( m_Values.size() );
	return this;
}

bool EPropertyCombo::OnCommand( EPropWnd* pParent , WPARAM wParam , LPARAM lParam )
{
	bool bPropertyChanged = false;

	int nBeg;
	int nEnd;
	pParent->GetIDRange( nBeg , nEnd );

	if( wParam == nBeg + 1 )
	{
		//
		// button clicked
		//

		int nNumSeparators = m_Separators.size();
		int iSeparator     = 0;
		int nNextSeparator = (iSeparator<nNumSeparators) ? m_Separators[iSeparator] : -1;

		CMenu menu;
		menu.CreatePopupMenu();

		int nCurrent = *(int*)m_pProperty;

		for( int i=0 ; i<m_StringArray.GetSize() ; i++ )
		{
			if( i==nNextSeparator )
			{
				menu.AppendMenu( MF_SEPARATOR );
				iSeparator++;
				i--;			//don't count string index for this
				nNextSeparator = (iSeparator<nNumSeparators) ? m_Separators[iSeparator] : -1;
			}
			else
			{
				int nFlags = MF_STRING | MF_ENABLED;

				if( m_Values[i] == nCurrent )
				{
					nFlags |= MF_CHECKED;
				}

				menu.AppendMenu( nFlags , nBeg + 2 + i , m_StringArray[i] );
			}
		}

		//
		//
		//
		
		ASSERT( m_pButton != NULL );

//-------- wont work when scrolled
//		CPoint p = m_ButtRect.CenterPoint();
//		pParent->ClientToScreen(&p);
//--------
		CRect r;
		m_pButton->GetWindowRect(&r);
		CPoint p = r.CenterPoint();
//--------

		menu.TrackPopupMenu(TPM_CENTERALIGN,p.x,p.y,pParent,NULL);
	}
	else if( (nBeg + 2 <= (int)wParam) && ((int)wParam <= nEnd) )		//combo menu selected
	{
		int i = wParam - ( nBeg + 2 );		//index of selected menu item
		int nSelected = m_Values[i];		//associated value (may be index itself..)
		int nCurrent  = *(int*)m_pProperty;
		if( nSelected != nCurrent )
		{
			if( m_pHost->PropertyChanging(m_pProperty,&nSelected) )
			{
				*(int*)m_pProperty = nSelected;
				bPropertyChanged = true;
			}
		}
	}

	return bPropertyChanged;
}




