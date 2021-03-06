
// TestCFormViewView.cpp : implementation of the CTestCFormViewView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestCFormView.h"
#endif

#include "TestCFormViewDoc.h"
#include "TestCFormViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestCFormViewView

IMPLEMENT_DYNCREATE(CTestCFormViewView, CView)

BEGIN_MESSAGE_MAP(CTestCFormViewView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTestCFormViewView construction/destruction

CTestCFormViewView::CTestCFormViewView() noexcept
{
	// TODO: add construction code here

}

CTestCFormViewView::~CTestCFormViewView()
{
}

BOOL CTestCFormViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTestCFormViewView drawing

void CTestCFormViewView::OnDraw(CDC* /*pDC*/)
{
	CTestCFormViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTestCFormViewView printing

BOOL CTestCFormViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestCFormViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestCFormViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTestCFormViewView diagnostics

#ifdef _DEBUG
void CTestCFormViewView::AssertValid() const
{
	CView::AssertValid();
}

void CTestCFormViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestCFormViewDoc* CTestCFormViewView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestCFormViewDoc)));
	return (CTestCFormViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestCFormViewView message handlers
