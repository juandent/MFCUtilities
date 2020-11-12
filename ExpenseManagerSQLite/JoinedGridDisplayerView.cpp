// JoinedGridDisplayerView.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "JoinedGridDisplayerView.h"

#include "TesterForMFCUtilitiesDLL/JoinedGridDisplayer.h"

using namespace sqlite_orm;
// using namespace Model;

// remove
struct GridStorage
{
	inline static auto& lines = Storage::getStorage().select(columns(&Transaccion::line_date, &Transaccion::amount_colones, &Transaccion::fkey_account_own, &Account::number,
		&Transaccion::fkey_category),
		inner_join<Account>(on(c(&Transaccion::fkey_account_own) == &Account::id_account)));

		
};


// JoinedGridDisplayerView

IMPLEMENT_DYNCREATE(JoinedGridDisplayerView, CFormView)

JoinedGridDisplayerView::JoinedGridDisplayerView()
	: CFormView(IDD_JoinedGridDisplayerView)
{

}

JoinedGridDisplayerView::~JoinedGridDisplayerView()
{
}

void JoinedGridDisplayerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_2, m_grid);
}

BEGIN_MESSAGE_MAP(JoinedGridDisplayerView, CFormView)
END_MESSAGE_MAP()


// JoinedGridDisplayerView diagnostics

#ifdef _DEBUG
void JoinedGridDisplayerView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void JoinedGridDisplayerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// JoinedGridDisplayerView message handlers


void JoinedGridDisplayerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	auto lines = Storage::getStorage().select(columns(&Transaccion::line_date, &Transaccion::amount_colones, &Transaccion::fkey_account_own, &Account::number,
		&Transaccion::fkey_category, &Categoria::name),
		inner_join<Account>(on(c(&Transaccion::fkey_account_own) == &Account::id_account)), inner_join<Categoria>(on(c(&Transaccion::fkey_category) == &Categoria::id_categoria)));


	
	std::vector<std::string> headers{ "LINE DATE", "AMOUNT LOCAL", "BELONGS TO", "ACCOUNT ID", "CATEGORY FID", "CATEGORY" };

	m_displayer.reset(new JoinedGridDisplayer<decltype(lines[0])> (m_grid, std::move(lines), std::move(headers)));
	m_displayer->display();

	// TODO: Add your specialized code here and/or call the base class
}
