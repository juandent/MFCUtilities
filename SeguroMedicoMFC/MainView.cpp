// MainView.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "MainView.h"
#include "Data.h"
#include "GridDisplayer.h"
#include <tuple>
#include "JoinedComboDisplayer.h"

// MainView

IMPLEMENT_DYNCREATE(MainView, CFormView)

MainView::MainView()
	: CFormView(IDD_MainView),
m_doctoresCB(m_lista_doctores, [](Doctor& doctor)
{
		return Util::to_cstring(doctor.simple_dump());
}),
m_pacientesCB(m_lista_pacientes, [](Patient& patient)
{
		return Util::to_cstring(patient.simple_dump());
}),
m_claimsCB(m_list_claim_ids, [](Claim& claim)
{
		return Util::to_cstring(claim.simple_dump());
})
{

}

MainView::~MainView()
{
}

void MainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_1, m_grid_1);
	DDX_Control(pDX, IDC_FECHA_INICIO, m_start_date);
	DDX_Control(pDX, IDC_FECHA_FINAL, m_end_date);
	DDX_Control(pDX, IDC_C_LISTA_DOCTORES, m_lista_doctores);
	DDX_Control(pDX, IDC_C_LISTA_PACIENTES, m_lista_pacientes);
	DDX_Control(pDX, IDC_C_FILTER_BY_DATES, m_filter_by_dates);
	DDX_Control(pDX, IDC_E_CLAIM_AMOUNT, m_claim_amount);
	DDX_Control(pDX, IDC_GRID_2, m_grid_2);
	DDX_Control(pDX, IDC_C_CLAIM_IDS, m_list_claim_ids);
}

BEGIN_MESSAGE_MAP(MainView, CFormView)
	ON_BN_CLICKED(IDC_B_FILTER, &MainView::OnBnClickedBFilter)
	ON_BN_CLICKED(IDC_B_FILTER_INSRESPONSES, &MainView::OnBnClickedBFilterInsresponses)
	ON_BN_CLICKED(IDC_B_CLEAR_FILTERS, &MainView::OnBnClickedBClearFilters)
	ON_BN_CLICKED(IDC_B_REFRESH, &MainView::OnBnClickedBRefresh)
END_MESSAGE_MAP()


// MainView diagnostics

#ifdef _DEBUG
void MainView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MainView message handlers


void MainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	SetAmount(m_claim_amount, 0.0);

	Refresh();
}

void MainView::Refresh()
{
	auto lines = Storage::getStorage().select(columns(alias_column<als_c>(&Claim::id),
		alias_column<als_c>(&Claim::id)));

	m_list_claim_ids.ResetContent();
	m_displayer_claims_combobox.reset(new JoinedComboDisplayer<decltype(lines[0])>(m_list_claim_ids, std::move(lines)));
	m_displayer_claims_combobox->display();

	m_doctoresCB.loadLBOrderBy(&Doctor::last_name);
	m_pacientesCB.loadLBOrderBy(&Patient::last_name);

	OnBnClickedBClearFilters();
	
	// OnBnClickedBFilter();
	// OnBnClickedBFilterInsresponses();
}


template<typename T>
void MainView::InitializeGridClaims(const T& t)
{
#if 0
	auto otherlines = Storage::getStorage().get_all<Patient>();

	std::vector<std::string> headers{ "ID", "FIRST NAME", "LAST NAME" };

	m_displayer_patients.reset( new GridDisplayer<Patient> (m_grid_1, std::move(otherlines), std::move(headers)));
	
	m_grid_1.m_sortingFunctions.push_back(Util::Comparison::Text);
	m_grid_1.m_sortingFunctions.push_back(Util::Comparison::Money);
	m_grid_1.m_sortingFunctions.push_back(Util::Comparison::Text);
	m_grid_1.m_sortingFunctions.push_back(Util::Comparison::Text);
	
	m_displayer_patients->display(&Patient::id, &Patient::first_name, &Patient::last_name);
#else


	
	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_c>(&Claim::id),
		alias_column<als_p>(&Patient::last_name),
		alias_column<als_d>(&Doctor::last_name),
		alias_column<als_d>(&Doctor::first_name),
		alias_column<als_c>(&Claim::start_date),
		alias_column<als_c>(&Claim::submission_date),
		alias_column<als_c>(&Claim::amount)),
		// alias_column<als_i>(&Invoice::fkey_INSResponse)),
		// sum(alias_column<als_i>(&Invoice::amount))),
		inner_join<als_p>(on(c(alias_column<als_p>(&Patient::id)) == alias_column<als_c>(&Claim::fkey_patient))),
		inner_join<als_d>(on(c(alias_column<als_d>(&Doctor::id)) == alias_column<als_c>(&Claim::fkey_doctor))),
		// inner_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)))
		where(t),
		order_by(alias_column<als_c>(&Claim::start_date)).desc());



	long count = otherlines.size();
	auto strCount = Util::to_cstring(count);
	// m_countMainGrid.SetWindowTextW(strCount);

	std::vector<std::string> headers{ "ID", "PATIENT", "DOCTOR LAST", "DOCTOR FIRST", "START DATE", "SUBMISSION DATE", "CLAIM AMOUNT" };

	m_displayer_claims.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<7>, IntegerList<0>>(m_grid_1, std::move(otherlines), std::move(headers))); // , ColonesFormat<14>{13}, DolaresFormat<14>{14}));
	m_displayer_claims->display();


#endif
}
template <class T>
void MainView::InitializeGridINSResponses(const T& t)
{
	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_k>(&INSResponseLine::id),
		alias_column<als_k>(&INSResponseLine::deducible_anual),
		alias_column<als_k>(&INSResponseLine::coaseguros),
		alias_column<als_k>(&INSResponseLine::copago),
		alias_column<als_k>(&INSResponseLine::monto_cubierto),
		alias_column<als_k>(&INSResponseLine::porcentaje_de_factura_cubierto),
		// alias_column<als_i>(&Invoice::id),
		alias_column<als_i>(&Invoice::number),
		alias_column<als_i>(&Invoice::amount),
		alias_column<als_i>(&Invoice::description),
		alias_column<als_c>(&Claim::id),
		alias_column<als_c>(&Claim::amount)),
		inner_join<als_i>(on(c(alias_column<als_k>(&INSResponseLine::fkey_factura)) == alias_column<als_i>(&Invoice::id))),
		inner_join<als_c>(on(c(alias_column<als_c>(&Claim::id)) == alias_column<als_i>(&Invoice::fkey_claim))),
		where(t),
		order_by(alias_column<als_c>(&Claim::start_date)).desc());

	std::vector<std::string> headers{ "ID", "DEDUCIBLE", "COASEGURO", "COPAGO", "MONTO CUBIERTO", "% DE FACTURA", "INV #", "INV AMNT", "DESC", "CLAIM ID", "CLAIM AMNT" };
	m_displayer_responses.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<8,11>, IntegerList<0>>(m_grid_2, std::move(otherlines), std::move(headers)));
	m_displayer_responses->display();
}



auto MainView::GetWhereStatement()
{
	auto claim = m_claimsCB.current();

	auto claimWhere = (not claim or (c(alias_column<als_c>(&Claim::id)) == claim->id));

	auto doctor = m_doctoresCB.current();
	auto paciente = m_pacientesCB.current();
	auto claim_amount_lower_bound = GetAmount(m_claim_amount);

	bool filter_dates = m_filter_by_dates.GetCheck();

	// TODO: Add your control notification handler code here
	auto start = GetDate(m_start_date);
	auto end = GetDate(m_end_date);

	auto whereStatement = (not filter_dates or (c(alias_column<als_c>(&Claim::start_date)) >= start
		&& c(alias_column<als_c>(&Claim::start_date)) <= end))
		&& (not doctor.has_value() or (c(alias_column<als_c>(&Claim::fkey_doctor)) == doctor->id))
		&& (not paciente.has_value() or (c(alias_column<als_c>(&Claim::fkey_patient)) == paciente->id))
		&& (not claim_amount_lower_bound > 0 or (c(alias_column<als_c>(&Claim::amount)) >= claim_amount_lower_bound))
		&& claimWhere;
	
	return whereStatement;
}

void MainView::OnBnClickedBFilter()
{

	// auto result_set = storage.get_all<StatementLine>(where((not filterCat or (c(&StatementLine::m_category_fid) == category)
	// 	&& (not filterConcept or c(&StatementLine::m_concept_fid) == concept)));
#if 0
	auto doctor = m_doctoresCB.current();
	auto paciente = m_pacientesCB.current();
	auto claim_amount_lower_bound = GetAmount(m_claim_amount);

	bool filter_dates = m_filter_by_dates.GetCheck();

	// TODO: Add your control notification handler code here
	auto start = GetDate(m_start_date);
	auto end = GetDate(m_end_date);

	auto whereStatement = (not filter_dates or (c(alias_column<als_c>(&Claim::start_date)) >= start
		&& c(alias_column<als_c>(&Claim::start_date)) <= end))
		&& (not doctor.has_value() or (c(alias_column<als_c>(&Claim::fkey_doctor)) == doctor->id))
		&& (not paciente.has_value() or (c(alias_column<als_c>(&Claim::fkey_patient)) == paciente->id))
		&& (not claim_amount_lower_bound > 0 or (c(alias_column<als_c>(&Claim::amount)) >= claim_amount_lower_bound));
#endif
	auto whereStatement = GetWhereStatement();
	InitializeGridClaims(whereStatement);
	InitializeGridINSResponses(whereStatement);
}


void MainView::OnBnClickedBFilterInsresponses()
{
	// TODO: Add your control notification handler code here
	auto whereStmt = GetWhereStatement();
	InitializeGridINSResponses(whereStmt);
}


void MainView::OnBnClickedBClearFilters()
{
	// TODO: Add your control notification handler code here
	m_claimsCB.select(std::nullopt);
	m_doctoresCB.select(std::nullopt);
	m_pacientesCB.select(std::nullopt);
	m_filter_by_dates.SetCheck(0);
	SetAmount(m_claim_amount, 0);
	OnBnClickedBFilter();
	// OnBnClickedBFilterInsresponses();
}



void MainView::OnBnClickedBRefresh()
{
	// TODO: Add your control notification handler code here
	Refresh();
}
