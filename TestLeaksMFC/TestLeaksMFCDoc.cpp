
// TestLeaksMFCDoc.cpp : implementation of the CTestLeaksMFCDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestLeaksMFC.h"
#endif

#include "TestLeaksMFCDoc.h"

#include <propkey.h>
#include "TestLeaksMFC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestLeaksMFCDoc

IMPLEMENT_DYNCREATE(CTestLeaksMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestLeaksMFCDoc, CDocument)
END_MESSAGE_MAP()


// CTestLeaksMFCDoc construction/destruction

CTestLeaksMFCDoc::CTestLeaksMFCDoc() noexcept
{
	// TODO: add one-time construction code here

}

CTestLeaksMFCDoc::~CTestLeaksMFCDoc()
{
}

BOOL CTestLeaksMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTestLeaksMFCDoc serialization

void CTestLeaksMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CTestLeaksMFCDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CTestLeaksMFCDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTestLeaksMFCDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTestLeaksMFCDoc diagnostics

#ifdef _DEBUG
void CTestLeaksMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestLeaksMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTestLeaksMFCDoc commands
