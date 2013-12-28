// this file is part of notepad++
// Copyright (C)2003 Harry <harrybharry@users.sourceforge.net>
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// Note that the GPL places important restrictions on "derived works", yet
// it does not provide a detailed definition of that term.  To avoid      
// misunderstandings, we consider an application to constitute a          
// "derivative work" for the purpose of this license if it does any of the
// following:                                                             
// 1. Integrates source code from Notepad++.
// 2. Integrates/includes/aggregates Notepad++ into a proprietary executable
//    installer, such as those produced by InstallShield.
// 3. Links to a library or executes a program that does any of the above.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "stdafx.h"
#include "SmartHighlighter.h"
#include "ScintillaDocView.h"
#include "SciLexer.h"
#include "common.h"

#define MAXLINEHIGHLIGHT 400	//prevent highlighter from doing too much work when a lot is visible

SmartHighlighter::SmartHighlighter(/*FindReplaceDlg * pFRDlg*/)
//: _pFRDlg(pFRDlg)
{
	//Nothing to do
}

void SmartHighlighter::highlightView(CScintillaView * pHighlightView)
{
	CScintillaCtrl& rCtrl = pHighlightView->GetCtrl();
	//Get selection
	CharacterRange range = pHighlightView->getSelection();

	//Clear marks
	//pHighlightView->clearIndicator(SCE_UNIVERSAL_FOUND_STYLE_SMART);

	//If nothing selected, dont mark anything
	if (range.cpMin == range.cpMax)
	{
		return;
	}

	int textlen = range.cpMax - range.cpMin + 1;

	char * text2Find = new char[textlen];
	pHighlightView->getSelectedText(text2Find, textlen, false);	//do not expand selection (false)

	bool valid = true;
	//The word has to consist if wordChars only, and the characters before and after something else
	if (!isQualifiedWord(text2Find))
		valid = false;
	else
	{
		UCHAR c = (UCHAR)rCtrl.GetCharAt(range.cpMax);
		if (c)
		{
			if (isWordChar(char(c)))
				valid = false;
		}
		c = (UCHAR)rCtrl.GetCharAt(range.cpMin-1);
		if (c)
		{
			if (isWordChar(char(c)))
				valid = false;
		}
	}
	if (!valid) {
		delete [] text2Find;
		return;
	}

	// save target locations for other search functions
	int originalStartPos = (int)rCtrl.GetTargetStart();
	int originalEndPos = (int)rCtrl.GetTargetEnd();

	// Get the range of text visible and highlight everything in it
	int firstLine =		(int)rCtrl.GetFirstVisibleLine();
	int nrLines =	min((int)rCtrl.LinesOnScreen(), MAXLINEHIGHLIGHT ) + 1;
	int lastLine =		firstLine+nrLines;
	int startPos =		0;
	int endPos =		0;
	int currentLine = firstLine;
	int prevDocLineChecked = -1;	//invalid start

	const TCHAR * searchText = NULL;

	unsigned int cp = rCtrl.GetCodePage(); 
	const TCHAR * text2FindW = char2wchar(text2Find, cp);
	searchText = text2FindW;

	//AfxMessageBox(searchText);

	for(; currentLine < lastLine; ++currentLine)
	{
		int docLine = (int)rCtrl.DocLineFromVisible(currentLine);
		if (docLine == prevDocLineChecked)
			continue;	//still on same line (wordwrap)
		prevDocLineChecked = docLine;
		startPos = (int)rCtrl.PositionFromLine(docLine);
		endPos = (int)rCtrl.PositionFromLine(docLine+1);
		
	}

	// restore the original targets to avoid conflicts with the search/replace functions
	rCtrl.SetTargetStart(originalStartPos);
	rCtrl.SetTargetEnd(originalEndPos);
}

bool SmartHighlighter::isQualifiedWord(const char *str) const
{
	for (size_t i = 0, len = strlen(str) ; i < len ; ++i)
	{
		if (!isWordChar(str[i]))
			return false;
	}
	return true;
};

bool SmartHighlighter::isWordChar(char ch) const
{
	if ((UCHAR)ch < 0x20) 
		return false;
	
	switch(ch)
	{
		case ' ':
		case '	':
		case '\n':
		case '\r':
		case '.':
		case ',':
		case '?':
		case ';':
		case ':':
		case '!':
		case '(':
		case ')':
		case '[':
		case ']':
		case '+':
		case '-':
		case '*':
		case '/':
		case '#':
		case '@':
		case '^':
		case '%':
		case '$':
		case '"':
		case '\'':
		case '~':
		case '&':
		case '{':
		case '}':
		case '|':
		case '=':
		case '<':
		case '>':
		case '\\':
			return false;
	}
	return true;
};
