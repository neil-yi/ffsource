// EPropertySystemTime.h: interface for the EPropertySystemTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPROPERTYSYSTEMTIME_H__F8643CB4_08BB_4A3F_81CE_1B277438092F__INCLUDED_)
#define AFX_EPROPERTYSYSTEMTIME_H__F8643CB4_08BB_4A3F_81CE_1B277438092F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EIconTextButtonProperty.h"

class GUILIBDLLEXPORT EPropertySystemTime : public EIconTextButtonProperty
{
public:
   
	EPropertySystemTime( IPropertyHost* pHost , const CString& sName , SYSTEMTIME* pProperty );
	~EPropertySystemTime();

	//
	// EIconTextButtonProperty
	//

	virtual CString GetTextValue();
};


#endif // !defined(AFX_EPROPERTYSYSTEMTIME_H__F8643CB4_08BB_4A3F_81CE_1B277438092F__INCLUDED_)
