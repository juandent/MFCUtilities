// ClaimsCollection.cpp : implementation file
//

#include "pch.h"
#include "SeguroMedicoMFCDocView.h"
#include "ClaimsCollection.h"


// ClaimsCollection

IMPLEMENT_DYNCREATE(ClaimsCollection, CDocument)

ClaimsCollection::ClaimsCollection()
{
}

BOOL ClaimsCollection::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

ClaimsCollection::~ClaimsCollection()
{
}


BEGIN_MESSAGE_MAP(ClaimsCollection, CDocument)
END_MESSAGE_MAP()


// ClaimsCollection diagnostics

#ifdef _DEBUG
void ClaimsCollection::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void ClaimsCollection::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// ClaimsCollection serialization

void ClaimsCollection::Serialize(CArchive& ar)
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


// ClaimsCollection commands
