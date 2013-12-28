// Gui_DevStudioDoc.cpp : implementation of the CGui_DevStudioDoc class
//

#include "stdafx.h"
#include "FFStudio.h"

#include "FFStudioDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioDoc

IMPLEMENT_DYNCREATE(CFFStudioDoc, CDocument)

BEGIN_MESSAGE_MAP(CFFStudioDoc, CDocument)
	//{{AFX_MSG_MAP(CGui_DevStudioDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioDoc construction/destruction

CFFStudioDoc::CFFStudioDoc()
{
	// TODO: add one-time construction code here

}

CFFStudioDoc::~CFFStudioDoc()
{
}

BOOL CFFStudioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioDoc diagnostics

#ifdef _DEBUG
void CFFStudioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFFStudioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioDoc commands
