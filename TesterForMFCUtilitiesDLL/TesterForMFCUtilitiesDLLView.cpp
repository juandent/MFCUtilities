
// TesterForMFCUtilitiesDLLView.cpp : implementation of the CTesterForMFCUtilitiesDLLView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TesterForMFCUtilitiesDLL.h"
#endif

#include "TesterForMFCUtilitiesDLLDoc.h"
#include "TesterForMFCUtilitiesDLLView.h"
#include "Repositories/StatementLineRepository.h"
#include "FilterStatementLines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Model;
using namespace std;
using namespace sqlite_orm;
//namespace View
//{




	// CTesterForMFCUtilitiesDLLView

	IMPLEMENT_DYNCREATE(CTesterForMFCUtilitiesDLLView, CFormView)

	BEGIN_MESSAGE_MAP(CTesterForMFCUtilitiesDLLView, CFormView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTesterForMFCUtilitiesDLLView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_NOTIFY(NM_CLICK, IDC_GRID_CTRL, OnGridClick)
		ON_BN_CLICKED(IDC_BUTTON_FILTER, &CTesterForMFCUtilitiesDLLView::OnBnClickedButtonFilter)
	END_MESSAGE_MAP()

	// CTesterForMFCUtilitiesDLLView construction/destruction

	CTesterForMFCUtilitiesDLLView::CTesterForMFCUtilitiesDLLView() noexcept
		: CFormView(IDD_TESTERFORMFCUTILITIESDLL_FORM)
	{
		// TODO: add construction code here

	}

	CTesterForMFCUtilitiesDLLView::~CTesterForMFCUtilitiesDLLView()
	{
	}

	void CTesterForMFCUtilitiesDLLView::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_GRID_CTRL, m_Category_grid);
		DDX_Control(pDX, IDC_GRID_CTRL2, m_statement_lines_grid);
	}

	BOOL CTesterForMFCUtilitiesDLLView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CFormView::PreCreateWindow(cs);
	}

	void CTesterForMFCUtilitiesDLLView::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();

		auto lines = ORM::storage.get_all<Category>(order_by(&Category::m_name_id));
		std::vector<std::string> headers{ "NAME", "REAL?" };
		m_Cat_grid_Displayer.reset(new GridDisplayer<Category>{ m_Category_grid, move(lines), move(headers) });
		m_Cat_grid_Displayer->display(&Category::m_name_id, &Category::m_real_expense_or_income); //->display(&Category::m_name_id, &Category::m_real_expense_or_income);

		OnBnClickedButtonFilter();
	}


	// CTesterForMFCUtilitiesDLLView printing


	void CTesterForMFCUtilitiesDLLView::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL CTesterForMFCUtilitiesDLLView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void CTesterForMFCUtilitiesDLLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void CTesterForMFCUtilitiesDLLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void CTesterForMFCUtilitiesDLLView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
	{
		// TODO: add customized printing code here
	}

	void CTesterForMFCUtilitiesDLLView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CTesterForMFCUtilitiesDLLView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CTesterForMFCUtilitiesDLLView diagnostics

#ifdef _DEBUG
	void CTesterForMFCUtilitiesDLLView::AssertValid() const
	{
		CFormView::AssertValid();
	}

	void CTesterForMFCUtilitiesDLLView::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}

	CTesterForMFCUtilitiesDLLDoc* CTesterForMFCUtilitiesDLLView::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTesterForMFCUtilitiesDLLDoc)));
		return (CTesterForMFCUtilitiesDLLDoc*)m_pDocument;
	}
#endif //_DEBUG



	void CTesterForMFCUtilitiesDLLView::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
	{
		//this->m_pCatGridController->OnGridClick(pNotifyStruct);
	}

	// CTesterForMFCUtilitiesDLLView message handlers

//}

	void CTesterForMFCUtilitiesDLLView::OnBnClickedButtonFilter()
	{
		// TODO: Add your control notification handler code here
		FilterStatementLines filterDialog{};
		auto ret = filterDialog.DoModal();
		if (ret != 1)	return;

		auto where_clause = filterDialog.get_where();

		using namespace sqlite_orm;
		using namespace Model;

		auto statement_lines = ORM::storage
			.select(columns(&StatementLine::m_statement_date,
				&StatementLine::m_belongs_to_account_fid,
				&StatementLine::m_lineDate,
				&StatementLine::m_concept_fid,
				&StatementLine::m_amountInLocal,
				&StatementLine::m_amountInDollars,
				&StatementLine::m_enabled,
				&StatementLine::m_category_fid),
				where(where_clause));

		auto headers2 = Queries::StatementLines_ForDate::headers;
		m_statement_lines_displayer.reset(new statement_lines_displayer_t
		(m_statement_lines_grid, move(statement_lines), move(headers2)));
		m_statement_lines_displayer->display();
		ResizeParentToFit();
	}
