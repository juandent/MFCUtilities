
// TesterForMFCUtilitiesDLLDoc.cpp : implementation of the CTesterForMFCUtilitiesDLLDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TesterForMFCUtilitiesDLL.h"
#endif

#include "TesterForMFCUtilitiesDLLDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//namespace Controller
//{

	// CTesterForMFCUtilitiesDLLDoc

	IMPLEMENT_DYNCREATE(CTesterForMFCUtilitiesDLLDoc, CDocument)

	BEGIN_MESSAGE_MAP(CTesterForMFCUtilitiesDLLDoc, CDocument)
	END_MESSAGE_MAP()


	// CTesterForMFCUtilitiesDLLDoc construction/destruction

	CTesterForMFCUtilitiesDLLDoc::CTesterForMFCUtilitiesDLLDoc() noexcept
	{
		// TODO: add one-time construction code here

	}

	CTesterForMFCUtilitiesDLLDoc::~CTesterForMFCUtilitiesDLLDoc()
	{
	}

	BOOL CTesterForMFCUtilitiesDLLDoc::OnNewDocument()
	{
		if (!CDocument::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		return TRUE;
	}




	// CTesterForMFCUtilitiesDLLDoc serialization

	void CTesterForMFCUtilitiesDLLDoc::Serialize(CArchive& ar)
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
	void CTesterForMFCUtilitiesDLLDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
	{
		// Modify this code to draw the document's data
		dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

		CString strText = _T("TODO: implement thumbnail drawing here");
		LOGFONT lf;

		CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
		pDefaultGUIFont->GetLogFont(&lf);
		lf.lfHeight = 36;

		CFont fontDraw;
		fontDraw.CreateFontIndirect(&lf);

		CFont* pOldFont = dc.SelectObject(&fontDraw);
		dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
		dc.SelectObject(pOldFont);
	}

	// Support for Search Handlers
	void CTesterForMFCUtilitiesDLLDoc::InitializeSearchContent()
	{
		CString strSearchContent;
		// Set search contents from document's data.
		// The content parts should be separated by ";"

		// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
		SetSearchContent(strSearchContent);
	}

	void CTesterForMFCUtilitiesDLLDoc::SetSearchContent(const CString& value)
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

	// CTesterForMFCUtilitiesDLLDoc diagnostics

#ifdef _DEBUG
	void CTesterForMFCUtilitiesDLLDoc::AssertValid() const
	{
		CDocument::AssertValid();
	}

	void CTesterForMFCUtilitiesDLLDoc::Dump(CDumpContext& dc) const
	{
		CDocument::Dump(dc);
	}
#endif //_DEBUG


	// CTesterForMFCUtilitiesDLLDoc commands
//}
