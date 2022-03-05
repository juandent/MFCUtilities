
// EZPrintView.cpp : implementation of the CEZPrintView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "EZPrint.h"
#endif

#include "EZPrintDoc.h"
#include "EZPrintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEZPrintView

IMPLEMENT_DYNCREATE(CEZPrintView, CView)

BEGIN_MESSAGE_MAP(CEZPrintView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEZPrintView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CEZPrintView construction/destruction

CEZPrintView::CEZPrintView() noexcept
{
	// TODO: add construction code here

}

CEZPrintView::~CEZPrintView()
{
}

BOOL CEZPrintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEZPrintView drawing

void CEZPrintView::OnDraw(CDC* /*pDC*/)
{
	CEZPrintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CEZPrintView printing


void CEZPrintView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEZPrintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEZPrintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEZPrintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CEZPrintView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEZPrintView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CEZPrintView diagnostics

#ifdef _DEBUG
void CEZPrintView::AssertValid() const
{
	CView::AssertValid();
}

void CEZPrintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEZPrintDoc* CEZPrintView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEZPrintDoc)));
	return (CEZPrintDoc*)m_pDocument;
}
#endif //_DEBUG


// CEZPrintView message handlers
