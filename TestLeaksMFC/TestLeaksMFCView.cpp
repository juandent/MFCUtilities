
// TestLeaksMFCView.cpp : implementation of the CTestLeaksMFCView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestLeaksMFC.h"
#endif

#include "TestLeaksMFCDoc.h"
#include "TestLeaksMFCView.h"
#include <afxpreviewviewex.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestLeaksMFCView

IMPLEMENT_DYNCREATE(CTestLeaksMFCView, CFormView)

BEGIN_MESSAGE_MAP(CTestLeaksMFCView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestLeaksMFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTestLeaksMFCView construction/destruction

CTestLeaksMFCView::CTestLeaksMFCView() noexcept
	: CFormView(IDD_TESTLEAKSMFC_FORM)
{
	// TODO: add construction code here

}

CTestLeaksMFCView::~CTestLeaksMFCView()
{
}

void CTestLeaksMFCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CTestLeaksMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTestLeaksMFCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

}


// CTestLeaksMFCView printing


void CTestLeaksMFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestLeaksMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestLeaksMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestLeaksMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTestLeaksMFCView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

void CTestLeaksMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTestLeaksMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTestLeaksMFCView diagnostics

#ifdef _DEBUG
void CTestLeaksMFCView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTestLeaksMFCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTestLeaksMFCDoc* CTestLeaksMFCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestLeaksMFCDoc)));
	return (CTestLeaksMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestLeaksMFCView message handlers
