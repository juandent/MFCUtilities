// JoinedGridDisplayerView.cpp : implementation file
//

#include "stdafx.h"

import Util;


#include "ExpenseManagerSQLite.h"
#include "JoinedGridDisplayerView.h"

#include "TesterForMFCUtilitiesDLL/JoinedGridDisplayer.h"
#include "JoinedComboDisplayer.h"

using namespace sqlite_orm;
// using namespace Model;

// struct OwnAccountAlias : alias_tag {
// 	static const std::string& get() {
// 		static const std::string res = "OWN ACCOUNT";
// 		return res;
// 	}
// };
//
// struct OtherAccountAlias : alias_tag {
// 	static const std::string& get() {
// 		static const std::string res = "OTHER ACCOUNT";
// 		return res;
// 	}
// };
//
//



// remove
struct GridStorage
{
	// inline static auto& lines = Storage::getStorage().select(columns(&Transaccion::line_date, &Transaccion::amount_colones, &Transaccion::fkey_account_own, &Account::number,
	// 	&Transaccion::fkey_category),
	// 	inner_join<Account>(on(c(&Transaccion::fkey_account_own) == &Account::id_account)));

		
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
	DDX_Control(pDX, IDC_C_STATEMENTDATES, m_statement_dates);
	DDX_Control(pDX, IDC_E_CONCEPTO, m_conceptoSearch);
	DDX_Control(pDX, IDC_E_COUNT, m_countMainGrid);
}

BEGIN_MESSAGE_MAP(JoinedGridDisplayerView, CFormView)
	ON_CBN_SELCHANGE(IDC_C_STATEMENTDATES, &JoinedGridDisplayerView::OnCbnSelchangeCStatementdates)
	// ON_EN_CHANGE(IDC_E_CONCEPTO, &JoinedGridDisplayerView::OnEnChangeEConcepto)
	// ON_EN_UPDATE(IDC_E_CONCEPTO, &JoinedGridDisplayerView::OnEnUpdateEConcepto)
	ON_EN_KILLFOCUS(IDC_E_CONCEPTO, &JoinedGridDisplayerView::OnEnKillfocusEConcepto)
	ON_BN_CLICKED(IDC_B_FILTER, &JoinedGridDisplayerView::OnBnClickedBFilter)
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
using als_t = alias_t<Transaccion>;
using als_a = alias_a<Account>;
using als_b = alias_b<Account>;
using als_c = alias_c<Categoria>;
using als_d = alias_d<Concepto>;
using als_s = alias_s<Statement>;
using als_o = alias_o<AccountOwner>;
using als_p = alias_p<AccountOwner>;


void JoinedGridDisplayerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();



	auto statements = Storage::getStorage().select(columns(			distinct(alias_column<als_t>(&Transaccion::fkey_statement)),
																				alias_column<als_s>(&Statement::date)),
		inner_join<als_s>(on(c(alias_column<als_t>(&Transaccion::fkey_statement)) == alias_column<als_s>(&Statement::id_statement))));


	m_combo_displayer.reset(new JoinedComboDisplayer<decltype(statements[0])>(m_statement_dates, std::move(statements)));
	m_combo_displayer->display();
}

template <typename T>
void JoinedGridDisplayerView::InitializeGrid(const T& t)
{


	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_t>(&Transaccion::id_transaccion),
		coalesce<int>(alias_column<als_t>(&Transaccion::fkey_account_other), -1),					// fkey_account_other int nullable
		// alias_column<als_t>(&Transaccion::fkey_account_own),										// fkey account own int
		coalesce<std::string>(alias_column<als_p>(&AccountOwner::name), ""),
		coalesce<std::string>(alias_column<als_a>(&Account::number), ""),							// fkey account other number nullable
		alias_column<als_d>(&Concepto::id_concepto),
		alias_column<als_d>(&Concepto::name),														// concepto::name
		alias_column<als_b>(&Account::number),														// fkey account own number
		alias_column<als_c>(&Categoria::name),														// categoria::name
		alias_column<als_s>(&Statement::date),														// statement::date
		alias_column<als_t>(&Transaccion::line_date),
		alias_column<als_o>(&AccountOwner::name),													// account owner:: name
		alias_column<als_s>(&Statement::id_statement),
		alias_column<als_t>(&Transaccion::amount_colones),
		alias_column<als_t>(&Transaccion::amount_dolares)),
		left_join<als_a>(on(c(alias_column<als_t>(&Transaccion::fkey_account_other)) == alias_column<als_a>(&Account::id_account))),
		left_join< als_b>(on(c(alias_column<als_t>(&Transaccion::fkey_account_own)) == alias_column<als_b>(&Account::id_account))),
		left_join< als_c>(on(c(alias_column<als_t>(&Transaccion::fkey_category)) == alias_column<als_c>(&Categoria::id_categoria))),
		left_join< als_d>(on(c(alias_column<als_t>(&Transaccion::fkey_concepto)) == alias_column<als_d>(&Concepto::id_concepto))),
		left_join< als_s>(on(c(alias_column<als_t>(&Transaccion::fkey_statement)) == alias_column<als_s>(&Statement::id_statement))),
		left_join<als_o>(on(c(alias_column<als_b>(&Account::fkey_account_owner)) == alias_column<als_o>(&AccountOwner::id_owner))),
		left_join<als_p>(on(c(alias_column<als_a>(&Account::fkey_account_owner)) == alias_column<als_p>(&AccountOwner::id_owner))),
		where(t));

	long count = otherlines.size();
	auto strCount = Util::to_cstring(count);
	m_countMainGrid.SetWindowTextW(strCount);
	
	std::vector<std::string> headers{ "TRANS ID", "ACCOUNT OTHER ID", "ACCOUNT OTHER OWNER", "ACCOUNT OTHER NUMBER", "CONCEPTO ID", "CONCEPTO", "ACCOUNT OWN NUMBER", "CATEGORIA", "STATEMENT DATE", "TRANS DATE", "OWN ACCOUNT OWNER NAME", "STATEMENT ID",  "COLONES", "DOLARES" }; // , "ACCOUNT ID", "CATEGORY FID", "CATEGORY", "STATEMENT DATE"


	m_displayer.reset(new JoinedGridDisplayer<decltype(otherlines[0])>(m_grid, std::move(otherlines), std::move(headers)));
	m_displayer->display();

}

void JoinedGridDisplayerView::OnCbnSelchangeCStatementdates()
{
	// TODO: Add your control notification handler code here
	const int index = m_statement_dates.GetCurSel();
	int fkey_statement = m_statement_dates.GetItemData(index);

	whereParameters.fkey_statement = fkey_statement;

	
	// auto whereClause = c(alias_column<als_t>(&Transaccion::fkey_statement)) == fkey_statement;


	// InitializeGrid(whereClause);
}


void JoinedGridDisplayerView::OnEnKillfocusEConcepto()
{
	// TODO: Add your control notification handler code here

	CString str;
	m_conceptoSearch.GetWindowTextW(str);

	auto concepto = Util::from_cstring(str);

	whereParameters.conceptoPattern = concepto;

	// auto whereClause = (like(alias_column<als_d>(&Concepto::name), concepto));


	// InitializeGrid(whereClause);

}

void JoinedGridDisplayerView::WhereParameters::executeWhere()
{
	auto fkey_statement = this->fkey_statement.value_or(1);

	auto whereStatement = c(alias_column<als_t>(&Transaccion::fkey_statement)) == fkey_statement;

	auto conceptoPattern = this->conceptoPattern.value_or("%");

	auto whereConcepto = like(alias_column<als_d>(&Concepto::name), conceptoPattern);

	auto whereClause = whereStatement && whereConcepto;

	view->InitializeGrid(whereClause);
}

void JoinedGridDisplayerView::OnBnClickedBFilter()
{
	// TODO: Add your control notification handler code here
	whereParameters.executeWhere();
}
