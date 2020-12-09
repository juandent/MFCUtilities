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
	DDX_Control(pDX, IDC_E_SUMCOLONES, m_sumColones);
	DDX_Control(pDX, IDC_E_SUMDOLARES, m_sumDollars);
	DDX_Control(pDX, IDC_E_CATEGORIA, m_categoriaFilter);
}

BEGIN_MESSAGE_MAP(JoinedGridDisplayerView, CFormView)
	ON_CBN_SELCHANGE(IDC_C_STATEMENTDATES, &JoinedGridDisplayerView::OnCbnSelchangeCStatementdates)
	// ON_EN_CHANGE(IDC_E_CONCEPTO, &JoinedGridDisplayerView::OnEnChangeEConcepto)
	// ON_EN_UPDATE(IDC_E_CONCEPTO, &JoinedGridDisplayerView::OnEnUpdateEConcepto)
	ON_EN_KILLFOCUS(IDC_E_CONCEPTO, &JoinedGridDisplayerView::OnEnKillfocusEConcepto)
	ON_BN_CLICKED(IDC_B_FILTER, &JoinedGridDisplayerView::OnBnClickedBFilter)
	ON_EN_KILLFOCUS(IDC_E_CATEGORIA, &JoinedGridDisplayerView::OnEnKillfocusECategoria)
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


	m_displayer.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<13>, IntegerList<14>>(m_grid, std::move(otherlines), std::move(headers))); // , ColonesFormat<14>{13}, DolaresFormat<14>{14}));
	m_displayer->display();

	auto sum_results = Storage::getStorage().select(columns(
		alias_column<als_t>(&Transaccion::id_transaccion), 
		alias_column<als_d>(&Concepto::id_concepto), 
		sum(alias_column<als_t>(&Transaccion::amount_colones)),
		sum(alias_column<als_t>(&Transaccion::amount_dolares))),
		left_join< als_d>(on(c(alias_column<als_t>(&Transaccion::fkey_concepto)) == alias_column<als_d>(&Concepto::id_concepto))), 
		left_join< als_c>(on(c(alias_column<als_t>(&Transaccion::fkey_category)) == alias_column<als_c>(&Categoria::id_categoria))),
		where(t));

	auto&& line = sum_results[0];
	auto&& pc = std::get<2>(line);

	Colones c(pc ? *pc : 0);
	auto ss = Util::to_cstring(c);
	
	m_sumColones.SetWindowTextW(ss);


	auto&& pd = std::get<3>(line);

	Dolares d(pd ? *pd : 0);
	auto dd = Util::to_cstring(d);
	
	m_sumDollars.SetWindowTextW(dd);

}



using namespace std;

#if 0
template<int N>
struct Integral
{
	static constexpr int Int = N;
};


template<int Value, typename List, int Index>
struct Find
{
	constexpr static bool same = std::get<Index>(List::Values).Int == Value;
	constexpr static bool Check()
	{
		// if same, found it - end recursion
		return same || Find<Value, List, Index - 1>::Check();	// not found at Index, try at Index - 1
	}
};

template<int Value, typename List>
struct Find<Value, List, 0>
{
	constexpr static bool same = std::get<0>(List::Values).Int == Value;
	constexpr static bool Check()
	{
		return same;
	}
};


/// <summary>
/// is Value in values list?
/// </summary>
template<int...N>
struct IntegerList
{
	constexpr inline static std::tuple<Integral<N>...> Values;
	constexpr static unsigned Count = std::tuple_size_v<decltype(Values)>;

	template<int Value>
	static bool found()
	{
		using lista = IntegerList;
		static bool found = Find<Value, lista, lista::Count - 1>::Check();
		return found;
	}
};


void useList()
{
	using lista = IntegerList<10, 13>;

	auto v = std::get<1>(lista::Values).Int;
	
	static bool same = std::get<1>(lista::Values).Int == 10;

	int s = same ? 1 : 50;

	static bool found = Find<10, lista, lista::Count - 1>::Check();

	if( found )
	{
		int s = 0;
		
	}

	bool f = IntegerList<10, 13, 15>::found<10>();
	f = IntegerList<10, 13, 15>::found<13>();
	f = IntegerList<10, 13, 15>::found<15>();
	f = IntegerList<10, 13, 15>::found<16>();
}
#endif


void JoinedGridDisplayerView::OnCbnSelchangeCStatementdates()
{
	// useList();

	
	// TODO: Add your control notification handler code here
	const int index = m_statement_dates.GetCurSel();
	int fkey_statement = m_statement_dates.GetItemData(index);

	whereParameters.fkey_statement = fkey_statement;
}


void JoinedGridDisplayerView::OnEnKillfocusEConcepto()
{
	// TODO: Add your control notification handler code here

	CString str;
	m_conceptoSearch.GetWindowTextW(str);

	std::string concepto = Util::from_cstring(str);

	if (concepto.empty())  concepto = "%";

	whereParameters.conceptoPattern = concepto;
}

void JoinedGridDisplayerView::WhereParameters::executeWhere()
{
	auto fkey_statement = this->fkey_statement.value_or(1);

	auto whereStatement = c(alias_column<als_t>(&Transaccion::fkey_statement)) == fkey_statement;

	auto conceptoPattern = this->conceptoPattern.value_or("%");

	auto whereConcepto = like(alias_column<als_d>(&Concepto::name), conceptoPattern);

	auto categoriaPattern = this->categoriaPattern.value_or("%");

	auto whereCategoria = like(alias_column<als_c>(&Categoria::name), categoriaPattern);

	
	auto whereClause = whereStatement && whereConcepto && whereCategoria;

	view->InitializeGrid(whereClause);
}

void JoinedGridDisplayerView::OnBnClickedBFilter()
{
	// TODO: Add your control notification handler code here
	whereParameters.executeWhere();
}


void JoinedGridDisplayerView::OnEnKillfocusECategoria()
{
	// TODO: Add your control notification handler code here
	CString str;
	m_categoriaFilter.GetWindowTextW(str);

	auto categoria = Util::from_cstring(str);

	if (categoria.empty()) categoria = "%";

	whereParameters.categoriaPattern = categoria;
}
