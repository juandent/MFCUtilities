
// SeguroMedicoView.cpp : implementation of the CSeguroMedicoView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SeguroMedico.h"
#endif

#include "SeguroMedicoDoc.h"
#include "SeguroMedicoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSeguroMedicoView

IMPLEMENT_DYNCREATE(CSeguroMedicoView, CView)

BEGIN_MESSAGE_MAP(CSeguroMedicoView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSeguroMedicoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSeguroMedicoView construction/destruction

CSeguroMedicoView::CSeguroMedicoView() noexcept
{
	// TODO: add construction code here

}

CSeguroMedicoView::~CSeguroMedicoView()
{
}

BOOL CSeguroMedicoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSeguroMedicoView drawing

void CSeguroMedicoView::OnDraw(CDC* /*pDC*/)
{
	CSeguroMedicoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSeguroMedicoView printing


void CSeguroMedicoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSeguroMedicoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSeguroMedicoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSeguroMedicoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSeguroMedicoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSeguroMedicoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSeguroMedicoView diagnostics

#ifdef _DEBUG
void CSeguroMedicoView::AssertValid() const
{
	CView::AssertValid();
}

void CSeguroMedicoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSeguroMedicoDoc* CSeguroMedicoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSeguroMedicoDoc)));
	return (CSeguroMedicoDoc*)m_pDocument;
}
#endif //_DEBUG


// CSeguroMedicoView message handlers
