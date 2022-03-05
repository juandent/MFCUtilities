
// ExpenseManagerSQLiteView.cpp : implementation of the CExpenseManagerSQLiteView class
//

#include "stdafx.h"


import Util;

#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ExpenseManagerSQLite.h"
#endif

#include "ExpenseManagerSQLiteDoc.h"
#include "ExpenseManagerSQLiteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExpenseManagerSQLiteView

IMPLEMENT_DYNCREATE(CExpenseManagerSQLiteView, CView)

BEGIN_MESSAGE_MAP(CExpenseManagerSQLiteView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CExpenseManagerSQLiteView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CExpenseManagerSQLiteView construction/destruction

CExpenseManagerSQLiteView::CExpenseManagerSQLiteView() noexcept
{
	// TODO: add construction code here

}

CExpenseManagerSQLiteView::~CExpenseManagerSQLiteView()
{
}

BOOL CExpenseManagerSQLiteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CExpenseManagerSQLiteView drawing

void CExpenseManagerSQLiteView::OnDraw(CDC* /*pDC*/)
{
	CExpenseManagerSQLiteDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CExpenseManagerSQLiteView printing


void CExpenseManagerSQLiteView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CExpenseManagerSQLiteView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CExpenseManagerSQLiteView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CExpenseManagerSQLiteView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CExpenseManagerSQLiteView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CExpenseManagerSQLiteView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CExpenseManagerSQLiteView diagnostics

#ifdef _DEBUG
void CExpenseManagerSQLiteView::AssertValid() const
{
	CView::AssertValid();
}

void CExpenseManagerSQLiteView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CExpenseManagerSQLiteDoc* CExpenseManagerSQLiteView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExpenseManagerSQLiteDoc)));
	return (CExpenseManagerSQLiteDoc*)m_pDocument;
}
#endif //_DEBUG


// CExpenseManagerSQLiteView message handlers
