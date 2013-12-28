#ifndef PROPERTY_VIEW_LIB_UTIL_H
#define PROPERTY_VIEW_LIB_UTIL_H

#define TAB_PANE_HEIGHT 30
#define COMMENT_SPLITTER_HEIGHT 8
#define COLUMN_HEADER_HEIGHT 13




#include <atlconv.h>

inline void Conv2Unicode(LPCTSTR tszDest, const char* szSrc)
{


#ifdef UNICODE
	
	USES_CONVERSION;
	tszDest = A2W( szSrc );  
#else
  tszDest = szSrc;  
#endif
}






int gGetColumnSeperatorWidth();
int gGetRowSeperatorWidth();
int gGetMargin();


class Tr
{
public:
	static bool g_bTraceCallTree;
	static DWORD s_nLastTraceTime;
	static CString s_sIndent;
	Tr( const TCHAR* c , const TCHAR* m )
	{
		if( g_bTraceCallTree==false )
		{
			return;
		}

		DWORD t = GetTickCount();
		
		if( t-s_nLastTraceTime > 500 )
		{
			TRACE( _T("-------------\n") );
		}

		s_nLastTraceTime = t;

		//
		//
		//

		CString s = s_sIndent;

		s += c;

		//
		// append spacing to value
		//

		if( s.GetLength() < 50 )
		{
			int n = 50 - s.GetLength();

			for( int i=0 ; i<n ; i++ )
			{
				s += ' ';
			}
		}
		else
		{
			s = s.Left(50);
		}

		s += m;
		s += "\n";

		//
		// append {
		//

		s += s_sIndent;
		s += "{\n";

		//
		//
		//

		TRACE(s);

		s_sIndent += _T("   ");
	}
	~Tr()
	{
		if( g_bTraceCallTree==false )
		{
			return;
		}

		s_sIndent = s_sIndent.Left( s_sIndent.GetLength() - 3 );

		//
		// trace }
		//

		CString s = s_sIndent;
		s +=  _T("}\n");
		TRACE(s);
	}
};

#ifdef _DEBUG
#define TR(a,b) Tr name_not_important(a,b);
#else
#define TR(a,b)
#endif

#endif