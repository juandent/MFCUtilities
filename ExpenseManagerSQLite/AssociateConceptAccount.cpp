// AssociateConceptAccount.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "AssociateConceptAccount.h"

namespace fs = std::filesystem;
// AssociateConceptAccount

IMPLEMENT_DYNCREATE(AssociateConceptAccount, CDocument)

AssociateConceptAccount::AssociateConceptAccount()
{
}

BOOL AssociateConceptAccount::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

AssociateConceptAccount::~AssociateConceptAccount()
{
}


BEGIN_MESSAGE_MAP(AssociateConceptAccount, CDocument)
	//ON_BN_CLICKED(IDC_B_LOAD_COMPOUND_DOC, &AssociateConceptAccount::OnBnClickedBLoadCompoundDoc)
END_MESSAGE_MAP()


// AssociateConceptAccount diagnostics

#ifdef _DEBUG
void AssociateConceptAccount::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void AssociateConceptAccount::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// AssociateConceptAccount serialization

void AssociateConceptAccount::Serialize(CArchive& ar)
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


// AssociateConceptAccount commands

