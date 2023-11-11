
// PasswordsView.cpp : implementation of the CPasswordsView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Passwords.h"
#endif

#include "PasswordsDoc.h"
#include "PasswordsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPasswordsView

IMPLEMENT_DYNCREATE(CPasswordsView, CView)

BEGIN_MESSAGE_MAP(CPasswordsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPasswordsView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CPasswordsView construction/destruction

CPasswordsView::CPasswordsView() noexcept
{
	// TODO: add construction code here

}

CPasswordsView::~CPasswordsView()
{
}

BOOL CPasswordsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPasswordsView drawing

void CPasswordsView::OnDraw(CDC* /*pDC*/)
{
	CPasswordsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CPasswordsView printing


void CPasswordsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPasswordsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPasswordsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPasswordsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPasswordsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPasswordsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPasswordsView diagnostics

#ifdef _DEBUG
void CPasswordsView::AssertValid() const
{
	CView::AssertValid();
}

void CPasswordsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPasswordsDoc* CPasswordsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPasswordsDoc)));
	return (CPasswordsDoc*)m_pDocument;
}
#endif //_DEBUG


// CPasswordsView message handlers
