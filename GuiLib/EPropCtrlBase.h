// EPropCtrlBase.h: interface for the EPropCtrlBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPROPCTRLBASE_H__60ACEE2B_146F_47E1_953D_CEAF27A028CB__INCLUDED_)
#define AFX_EPROPCTRLBASE_H__60ACEE2B_146F_47E1_953D_CEAF27A028CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GUILIBDLLEXPORT EPropCtrlBase : public CWnd
{

	DECLARE_MESSAGE_MAP();
	DECLARE_DYNCREATE(EPropCtrlBase);

public:
	EPropCtrlBase();
	virtual ~EPropCtrlBase();

};

#endif // !defined(AFX_EPROPCTRLBASE_H__60ACEE2B_146F_47E1_953D_CEAF27A028CB__INCLUDED_)
