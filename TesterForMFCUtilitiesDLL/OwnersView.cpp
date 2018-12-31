// OwnersView.cpp : implementation file
//

#include "stdafx.h"
#include "TesterForMFCUtilitiesDLL.h"
#include "OwnersView.h"


// OwnersView

IMPLEMENT_DYNCREATE(OwnersView, CFormView)

OwnersView::OwnersView()	 : CFormView(IDD)
{

}

OwnersView::~OwnersView()
{
}

BEGIN_MESSAGE_MAP(OwnersView, CFormView)
	// Standard Printing Commands
	ON_COMMAND(ID_FILE_PRINT,&CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &OwnersView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// OwnersView drawing

void OwnersView::OnDraw(CDC* pDC)
{
	CFormView::OnDraw(pDC);
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// OwnersView diagnostics

#ifdef _DEBUG
void OwnersView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void OwnersView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// OwnersView message handlers
void OwnersView::DoDataExchange(CDataExchange* pDC)
{
	CFormView::DoDataExchange(pDC);
	DDX_Control(pDC, IDC_GRID_OWNERS, m_grid);
}
BOOL OwnersView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}
void OwnersView::OnInitialUpdate()
{
	using namespace Model;
	using namespace sqlite_orm;

	CFormView::OnInitialUpdate();


	auto lines = ORM::storage.get_all<Person>(order_by(&Person::m_first_name));
	std::vector<std::string> headers{ "PERS ID", "FIRST NAME", "LAST NAME", "COMPANY" };

	m_grid_Displayer.reset(new GridDisplayer<Person>{ m_grid, move(lines), move(headers) });
	m_grid_Displayer->display(&Person::m_id,
		&Person::m_first_name,
		&Person::m_last_name,
		&Person::m_company_name);

	//m_grid.SetRowCount(10);
	//m_grid.SetColumnCount(4);
	//m_grid.SetFixedRowCount(1);
	//m_grid.SetFixedColumnCount(1);
	//m_grid.SetHeaderSort(true);
	//m_grid.SetSingleRowSelection(true);


	ResizeParentToFit();
}
void OwnersView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}
BOOL OwnersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
void OwnersView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing;
}
void OwnersView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing;
}
void OwnersView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add customized printing code here;
}
void OwnersView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}
void OwnersView::OnContextMenu(CWnd* pWnd, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, true);
#endif
}
CDocument* OwnersView::GetDocument() const
{
	return m_pDocument;
}
