// JoinedGridDisplayerDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "JoinedGridDisplayerDoc.h"


// JoinedGridDisplayerDoc

IMPLEMENT_DYNCREATE(JoinedGridDisplayerDoc, CDocument)

JoinedGridDisplayerDoc::JoinedGridDisplayerDoc()
{
}

BOOL JoinedGridDisplayerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

JoinedGridDisplayerDoc::~JoinedGridDisplayerDoc()
{
}


BEGIN_MESSAGE_MAP(JoinedGridDisplayerDoc, CDocument)
END_MESSAGE_MAP()


// JoinedGridDisplayerDoc diagnostics

#ifdef _DEBUG
void JoinedGridDisplayerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void JoinedGridDisplayerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// JoinedGridDisplayerDoc serialization

void JoinedGridDisplayerDoc::Serialize(CArchive& ar)
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


// JoinedGridDisplayerDoc commands
