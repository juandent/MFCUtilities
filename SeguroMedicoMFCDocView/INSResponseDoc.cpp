// INSResponseDoc.cpp : implementation file
//

#include "pch.h"
#include "SeguroMedicoMFCDocView.h"
#include "INSResponseDoc.h"


// INSResponseDoc

IMPLEMENT_DYNCREATE(INSResponseDoc, CDocument)

INSResponseDoc::INSResponseDoc()
{
}

BOOL INSResponseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

INSResponseDoc::~INSResponseDoc()
{
}


BEGIN_MESSAGE_MAP(INSResponseDoc, CDocument)
END_MESSAGE_MAP()


// INSResponseDoc diagnostics

#ifdef _DEBUG
void INSResponseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void INSResponseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// INSResponseDoc serialization

void INSResponseDoc::Serialize(CArchive& ar)
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


// INSResponseDoc commands
