// Util.cpp: implementation of the Util class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int gGetColumnSeperatorWidth()
{
	return 1;
}

int gGetRowSeperatorWidth()
{
	return 0;
}

int gGetMargin()
{
	return 4;
}

CString Tr::s_sIndent;
DWORD Tr::s_nLastTraceTime = 0;
bool Tr::g_bTraceCallTree = false;
