// Gui_DevStudioView.cpp : implementation of the CGui_DevStudioView class
//

#include "stdafx.h"
#include "FFStudio.h"

#include "FFStudioDoc.h"
#include "FFStudioView.h"
#include "NewMenu.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioView

IMPLEMENT_DYNCREATE(CFFStudioView, CScintillaView)

BEGIN_MESSAGE_MAP(CFFStudioView, CScintillaView)
	//{{AFX_MSG_MAP(CGui_DevStudioView)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScintillaView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScintillaView::OnFilePrint)

END_MESSAGE_MAP()


const TCHAR cppKeyWords[] = 
  _T("and and_eq asm auto bitand bitor bool break ")
  _T("case catch char class compl const const_cast continue ")
  _T("default delete do double dynamic_cast else enum explicit export extern false float for ")
  _T("friend goto if inline int long mutable namespace new not not_eq ")
  _T("operator or or_eq private protected public ")
  _T("register reinterpret_cast return short signed sizeof static static_cast struct switch ")
  _T("template this throw true try typedef typeid typename union unsigned using ")
  _T("virtual void volatile wchar_t while xor xor_eq ");

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioView construction/destruction

CFFStudioView::CFFStudioView()
{
	// TODO: add construction code here

}

CFFStudioView::~CFFStudioView()
{
}

BOOL CFFStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CScintillaView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioView drawing

void CFFStudioView::OnDraw(CDC* pDC)
{
	CFFStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioView printing

BOOL CFFStudioView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CScintillaView::OnPreparePrinting(pInfo);
}

void CFFStudioView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CScintillaView::OnBeginPrinting(pDC, pInfo);
}

void CFFStudioView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CScintillaView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioView diagnostics

#ifdef _DEBUG
/*void CGui_DevStudioView::AssertValid() const
{
	CEditView::AssertValid();
}

void CGui_DevStudioView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
*/
CFFStudioDoc* CFFStudioView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFFStudioDoc)));
	return (CFFStudioDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGui_DevStudioView message handlers
static CString GetShort(UINT id)
{
	CString str;
	str.LoadString(id);
	int nIndex = str.ReverseFind(_T('\n'));
	if(nIndex!=-1)
	{
		str=str.Mid(nIndex+1);
	}
	return str;
}


void CFFStudioView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if(point.x==-1 && point.y==-1)
	{
		CRect rect;
		GetWindowRect(rect);

		point = rect.TopLeft()+CPoint(10,10);
	}
	SetFocus();

	CMenu menu;
	menu.CreatePopupMenu();
	menu.InsertMenu(0, MF_BYPOSITION , ID_EDIT_UNDO, GetShort(ID_EDIT_UNDO));
	menu.InsertMenu(1, MF_BYPOSITION|MF_SEPARATOR);
	menu.InsertMenu(2, MF_BYPOSITION , ID_EDIT_COPY, GetShort(ID_EDIT_COPY));
	menu.InsertMenu(2, MF_BYPOSITION , ID_EDIT_CUT, GetShort(ID_EDIT_CUT));
	menu.InsertMenu(4, MF_BYPOSITION , ID_EDIT_CLEAR, GetShort(ID_EDIT_CLEAR));
	menu.InsertMenu(5, MF_BYPOSITION , ID_EDIT_PASTE, GetShort(ID_EDIT_PASTE));
	menu.InsertMenu(6, MF_BYPOSITION|MF_SEPARATOR);

	menu.InsertMenu(7, MF_BYPOSITION , ID_EDIT_SELECT_ALL,GetShort(ID_EDIT_SELECT_ALL));

//	menu.SetMenuTitle(_T("Edit commands"),MFT_GRADIENT|MFT_SIDE_TITLE|MFT_LINE);
	//menu.Lo.LoadToolBar(IDR_MAINFRAME);

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}

void CFFStudioView::OnInitialUpdate()
{
	  //Let the base class do its thing
  CScintillaView::OnInitialUpdate();
  
  CScintillaCtrl& rCtrl = GetCtrl();

  //Setup the Lexer
  rCtrl.SetLexer(SCLEX_CPP);
  rCtrl.SetKeyWords(0, cppKeyWords);

  //Setup styles
  SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 11, "Verdana");
  rCtrl.StyleClearAll();
  SetAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
  SetAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_NUMBER, RGB(0, 0x80, 0x80));
  SetAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
  rCtrl.StyleSetBold(SCE_C_WORD, 1);
  SetAStyle(SCE_C_STRING, RGB(0x80, 0, 0x80));
  SetAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
  SetAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
  SetAStyle(SCE_C_OPERATOR, RGB(0x80, 0x80, 0));

  //Setup folding
  rCtrl.SetMarginWidthN(2, 16);
  rCtrl.SetMarginSensitiveN(2, TRUE);
  rCtrl.SetMarginTypeN(2, SC_MARGIN_SYMBOL);
  rCtrl.SetMarginMaskN(2, SC_MASK_FOLDERS);
  rCtrl.SetProperty(_T("fold"), _T("1"));

  //Setup markers
  DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
  DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_PLUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));

  //Setup auto completion
  rCtrl.AutoCSetSeparator(10); //Use a separator of line feed

  //Setup call tips
  rCtrl.SetMouseDwellTime(1000);

  //Enable Multiple selection
  rCtrl.SetMultipleSelection(TRUE);

  //Disable scintilla's context menu
  rCtrl.UsePopUp(FALSE);
}

void CFFStudioView::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face) 
{
  CScintillaCtrl& rCtrl = GetCtrl();

  rCtrl.StyleSetFore(style, fore);
  rCtrl.StyleSetBack(style, back);
  if (size >= 1)
    rCtrl.StyleSetSize(style, size);
  if (face) 
    rCtrl.StyleSetFont(style, face);
}

void CFFStudioView::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back) 
{
  CScintillaCtrl& rCtrl = GetCtrl();

  rCtrl.MarkerDefine(marker, markerType);
  rCtrl.MarkerSetFore(marker, fore);
  rCtrl.MarkerSetBack(marker, back);
}


