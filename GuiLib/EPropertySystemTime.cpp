// EPropertySystemTime.cpp: implementation of the EPropertySystemTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EPropertySystemTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EPropertySystemTime::EPropertySystemTime( IPropertyHost* pHost , const CString& sName , SYSTEMTIME* pProperty ) : EIconTextButtonProperty( pHost , sName , pProperty , false )
{
}

EPropertySystemTime::~EPropertySystemTime()
{
}

CString EPropertySystemTime::GetTextValue()
{
	SYSTEMTIME* pST = (SYSTEMTIME*)m_pProperty;
	CString s;
	s.Format( _T("%4d-%02d-%02d %02d:%02d"),pST->wYear,pST->wMonth,pST->wDay,pST->wHour,pST->wMinute);
	return s;
}
