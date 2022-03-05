// SubFormView.cpp : implementation file
//

#include "pch.h"
#include "TestCFormView.h"
#include "SubFormView.h"


// SubFormView

IMPLEMENT_DYNCREATE(SubFormView, CFormView)

SubFormView::SubFormView()
	: CFormView(IDD_SubFormView)
{

}

SubFormView::~SubFormView()
{
}

void SubFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(SubFormView, CFormView)
END_MESSAGE_MAP()


// SubFormView diagnostics

#ifdef _DEBUG
void SubFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void SubFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// SubFormView message handlers
