// MainViewDoc.cpp : implementation file
//

#include "pch.h"
#include "SeguroMedico.h"
#include "MainViewDoc.h"


// MainViewDoc

IMPLEMENT_DYNCREATE(MainViewDoc, CDocument)

MainViewDoc::MainViewDoc()
{
}

BOOL MainViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

MainViewDoc::~MainViewDoc()
{
}


BEGIN_MESSAGE_MAP(MainViewDoc, CDocument)
END_MESSAGE_MAP()


// MainViewDoc diagnostics

#ifdef _DEBUG
void MainViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void MainViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// MainViewDoc serialization

void MainViewDoc::Serialize(CArchive& ar)
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
#endif


// MainViewDoc commands
