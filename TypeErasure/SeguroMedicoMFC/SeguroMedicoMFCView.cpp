
// SeguroMedicoMFCView.cpp : implementation of the CSeguroMedicoMFCView class
//

#include "stdafx.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SeguroMedicoMFC.h"
#endif

#include "SeguroMedicoMFCDoc.h"
#include "SeguroMedicoMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSeguroMedicoMFCView

IMPLEMENT_DYNCREATE(CSeguroMedicoMFCView, CView)

BEGIN_MESSAGE_MAP(CSeguroMedicoMFCView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSeguroMedicoMFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSeguroMedicoMFCView construction/destruction

CSeguroMedicoMFCView::CSeguroMedicoMFCView() noexcept
{
	// TODO: add construction code here

}

CSeguroMedicoMFCView::~CSeguroMedicoMFCView()
{
}

BOOL CSeguroMedicoMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSeguroMedicoMFCView drawing

void CSeguroMedicoMFCView::OnDraw(CDC* /*pDC*/)
{
	CSeguroMedicoMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSeguroMedicoMFCView printing


void CSeguroMedicoMFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSeguroMedicoMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSeguroMedicoMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSeguroMedicoMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSeguroMedicoMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSeguroMedicoMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSeguroMedicoMFCView diagnostics

#ifdef _DEBUG
void CSeguroMedicoMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CSeguroMedicoMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSeguroMedicoMFCDoc* CSeguroMedicoMFCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSeguroMedicoMFCDoc)));
	return (CSeguroMedicoMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CSeguroMedicoMFCView message handlers
