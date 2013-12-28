/* Generated by Together */

#ifndef EPROPERTYCOMBO_H
#define EPROPERTYCOMBO_H

#include "EIconTextButtonProperty.h"
#include <vector>

class EPropListCtrl;
class IPropertyHost;

class GUILIBDLLEXPORT EPropertyCombo : public EIconTextButtonProperty
{

protected:

	CStringArray m_StringArray;
	std::vector<int> m_Values;
	std::vector<int> m_Separators;

public:

	EPropertyCombo( IPropertyHost* pHost , const CString& sName , int* pProperty );

	virtual bool OnCommand( EPropWnd* pParent , WPARAM wParam , LPARAM lParam );
	virtual bool HasButton();
	virtual CString GetTextValue();

	EPropertyCombo* AddString( const CString& s );
	EPropertyCombo* AddString( const CString& s , int nValue );

	EPropertyCombo* AddSeparator();

};
#endif //EPROPERTYCOMBO_H