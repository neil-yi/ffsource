/* Generated by Together */

#ifndef EPropList_H
#define EPropList_H

#include <vector>
#include <stack>

class EProperty;
class IPropertyHost;
class EPropertyDouble;
class EPropertyFloat;
class EPropertyInt;
class EPropertyCombo;
class EPropertyString;
class EPropertyFile;
class EPropertyDir;
class EPropertyColor;
class EPropertyCheck;
class EPropertyCharString;
class EPropertyFont;
class EPropertyTextblock;
class EPropertySeparator;
class EPropertySystemTime;

class GUILIBDLLEXPORT EPropList
{

public:

	class ETabGroup
	{
	private:
		CString m_sName;
		int m_nIcon;
		std::vector<EProperty*> m_vProperties;
	public:
		ETabGroup( const CString& sName , int nIcon );
		~ETabGroup();
		void AddProperty(EProperty* pProperty);
		int Size();
		CString* GetName();
		int GetIcon();
		EProperty* Get(int i);
	};

protected:

	int m_nCurrentTab;
	std::vector<ETabGroup*> m_vTabGroups;
	bool m_bHasUserTabs;
	int m_nIndent;
	std::stack<IPropertyHost*> m_vParentStack;
	CString m_sChildHostName;	//if an objects adds child hosts, this is the name of the child (children can get this value and create property key names using this)

public:

	CString m_sHostComment;

public:    

	EPropList();
	virtual ~EPropList();

	int        Size();
	EProperty* Get(int i);
	void       Clear();
	int        GetActiveTab();
	void       SetActiveTab(int i);
	int        GetTabCount();
	ETabGroup* GetTab(int i);
	bool       HasUserTabs();
	EProperty* GetProperty( const CString& sName );
	void       StepIndentIn();
	void       StepIndentOut();
	CString    GetChildHostName();

	//
	// this is for propertyhost to see (maybe put these in a prent class so other methods are unavailable for property hosts
	//

	virtual EProperty*           AddProperty      ( EProperty * pProperty );
	virtual EPropertyInt*        AddPropInt       ( IPropertyHost* pHost , const CString& sName , void*     pProperty , const CString& sUnit="" , bool bEnabled=true );
	virtual EPropertyFloat*      AddPropFloat     ( IPropertyHost* pHost , const CString& sName , float*    pFloat    , const CString& sUnit="" , bool bEnabled=true );
	virtual EPropertyDouble*     AddPropDouble    ( IPropertyHost* pHost , const CString& sName , double*   pDouble   , const CString& sUnit="" , bool bEnabled=true );
	virtual EPropertyCheck*      AddPropCheck     ( IPropertyHost* pHost , const CString& sName , bool*     pBoolean );
	virtual EPropertyString*     AddPropString    ( IPropertyHost* pHost , const CString& sName , CString*  pString , bool bEnabled=true );
	virtual EPropertyCharString* AddPropCharString( IPropertyHost* pHost , const CString& sName , char*     pString , int nLength=0 );
	virtual EPropertyFont*       AddPropFont      ( IPropertyHost* pHost , const CString& sName , CFont*    pFont );
	virtual EPropertyColor*      AddPropColor     ( IPropertyHost* pHost , const CString& sName , COLORREF* pColor , bool bEnabled= true );
	virtual EPropertyTextblock*  AddPropTextblock ( IPropertyHost* pHost , const CString& sName , CString*  pText , bool bEnabled=true );
	virtual EPropertyCombo*      AddPropCombo     ( IPropertyHost* pHost , const CString& sName , int*      pComboIndex );
	virtual EPropertyFile*       AddPropFile      ( IPropertyHost* pHost , const CString& sName , CString*  pFilename , const CString& sFilters = "All Files (*.*)|*.*||" );
	virtual EPropertyDir*        AddPropDir       ( IPropertyHost* pHost , const CString& sName , CString*  pDir );
	virtual EPropertySystemTime* AddPropSystemTime( IPropertyHost* pHost , const CString& sName , SYSTEMTIME* pSystemTime );
	virtual EPropertySeparator*  AddPropSeparator ( IPropertyHost* pHost , const CString& sName );
	virtual void                 AddPropHost      ( IPropertyHost* pHost , IPropertyHost* pChildHost );
	virtual void                 AddPropHost      ( IPropertyHost* pHost , const CString& sName , IPropertyHost* pChildHost );
	virtual void                 AddTab           ( const CString& sName , int nIcon=-1 );	//-1=none, to use icons call SetImageList()
	virtual void                 SetComment       ( const CString& sHostComment );
};
#endif //EPropList_H