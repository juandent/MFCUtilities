// OwnerDoc.cpp : implementation file
//

#include "stdafx.h"
#include "TesterForMFCUtilitiesDLL.h"
#include "OwnerDoc.h"


// OwnerDoc

IMPLEMENT_DYNCREATE(OwnerDoc, CDocument)

OwnerDoc::OwnerDoc()
{
}

BOOL OwnerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

OwnerDoc::~OwnerDoc()
{
}


BEGIN_MESSAGE_MAP(OwnerDoc, CDocument)
END_MESSAGE_MAP()


// OwnerDoc diagnostics

#ifdef _DEBUG
void OwnerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void OwnerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// OwnerDoc serialization

void OwnerDoc::Serialize(CArchive& ar)
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


// OwnerDoc commands
