// OwnersView.cpp : implementation file
//

#include "stdafx.h"
#include "TesterForMFCUtilitiesDLL.h"
#include "OwnersView.h"


// OwnersView

IMPLEMENT_DYNCREATE(OwnersView, CView)

OwnersView::OwnersView()
{

}

OwnersView::~OwnersView()
{
}

BEGIN_MESSAGE_MAP(OwnersView, CView)
END_MESSAGE_MAP()


// OwnersView drawing

void OwnersView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// OwnersView diagnostics

#ifdef _DEBUG
void OwnersView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void OwnersView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// OwnersView message handlers
