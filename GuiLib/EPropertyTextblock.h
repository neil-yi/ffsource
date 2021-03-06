/* Generated by Together */

#ifndef EPROPERTYTEXTBLOCK_H
#define EPROPERTYTEXTBLOCK_H


#include "EIconTextButtonProperty.h"

class IPropertyHost;
class EPropertyTextblockWindow;

class GUILIBDLLEXPORT EPropertyTextblock : public EIconTextButtonProperty
{

protected:

	EPropertyTextblockWindow* m_pPropertyEditWindow;
	EPropWnd* m_pCtrl;
	CFont*    m_pEditWindowFont;

	void CloseEditWindow();

public:

	//
	// construction/destruction
	//
    EPropertyTextblock( IPropertyHost* pHost , const CString& sName , CString* pProperty , bool bEnabled );
	~EPropertyTextblock();

	//
	// EProperty overrides
	//
	virtual bool OnCommand( EPropWnd* pParent , WPARAM wParam , LPARAM lParam );
	virtual void OnPropClose();
	virtual bool OnPropApply();

	//
	// EIconTextButtonProperty
	//
	virtual bool HasButton();
	virtual CString GetTextValue();

	//
	// this
	//
	void OnWindowButtonCancel();
	void OnWindowButtonOk();

	//
	//
	//

	EPropertyTextblock* SetEditWindowFont( CFont* pFont );

};

#endif //EPROPERTYSTRING_H