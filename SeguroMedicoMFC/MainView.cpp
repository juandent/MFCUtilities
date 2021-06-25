// MainView.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "MainView.h"
#include "Data.h"
#include "..\TesterForMFCUtilitiesDLL/GridDisplayer.h"
#include <tuple>


#include "ClaimDlg.h"
#include "InvoiceDlg.h"
#include "..\ExpenseManagerSQLite/JoinedComboDisplayer.h"

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
	DDX_Control(pDX, IDC_E_NUM_FACTURA, m_num_Factura);
	DDX_Control(pDX, IDC_E_DESC, m_invoice_description);
	DDX_Control(pDX, IDC_E_CLAIM_AMOUNT_LESS_THAN, m_claim_amount_less_than);
}

BEGIN_MESSAGE_MAP(MainView, CFormView)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_2, OnGrid2StartSelChange)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_1, OnGrid1StartSelChange)
	ON_BN_CLICKED(IDC_B_FILTER, &MainView::OnBnClickedBFilter)
	ON_BN_CLICKED(IDC_B_FILTER_INSRESPONSES, &MainView::OnBnClickedBFilterInsresponses)
	ON_BN_CLICKED(IDC_B_CLEAR_FILTERS, &MainView::OnBnClickedBClearFilters)
	ON_BN_CLICKED(IDC_B_REFRESH, &MainView::OnBnClickedBRefresh)
	ON_COMMAND(ID_FILE_PRINT, &MainView::OnFilePrint)
	// ON_BN_CLICKED(IDC_B_SELECT_REEMBOLSO, &MainView::OnBnClickedBSelectReembolso)
	ON_BN_CLICKED(IDC_B_CREAR_REEMBOLSO, &MainView::OnBnClickedBCrearReembolso)
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

struct response_1_count : alias_tag {
	static const std::string& get() {
		static const std::string res = "response_1_count";
		return res;
	}
};

struct ii : alias_tag {
	static const std::string& get() {
		static const std::string res = "ii";
		return res;
	}
};


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

// #define NESTED_SELECT
#ifndef  NESTED_SELECT
#if 1
	auto otherlines = Storage::getStorage().select(columns(
		// distinct(alias_column<als_c>(&Claim::id)),
		alias_column<als_c>(&Claim::id),
		alias_column<als_p>(&Patient::last_name),
		conc(conc(alias_column<als_d>(&Doctor::last_name), " "), alias_column<als_d>(&Doctor::first_name)),
		alias_column<als_c>(&Claim::status),
		alias_column<als_c>(&Claim::start_date),
		alias_column<als_c>(&Claim::submission_date),
		alias_column<als_c>(&Claim::amount),
		alias_column<als_c>(&Claim::asprose_claim_number),
		alias_column<als_m>(&Medication::name),
		// alias_column<als_i>(&Invoice::fkey_INSResponse),
		select(greater_than(count<als_q>(), 0), from<als_q>(),
			where(c(alias_column<als_q>(&Invoice::fkey_INSResponse)) == 1 && c(alias_column<als_q>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)))),
		// sum(alias_column<als_i>(&Invoice::amount))),
		inner_join<als_p>(on(c(alias_column<als_p>(&Patient::id)) == alias_column<als_c>(&Claim::fkey_patient))),
		inner_join<als_d>(on(c(alias_column<als_d>(&Doctor::id)) == alias_column<als_c>(&Claim::fkey_doctor))),
		inner_join<als_m>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),
		// inner_join<als_c>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),
		inner_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))),
		where(t),
		group_by(alias_column<als_c>(&Claim::id)));
		// order_by(alias_column<als_c>(&Claim::submission_date)).desc());

#else
	auto otherlines = Storage::getStorage().select(columns(
		distinct(alias_column<als_c>(&Claim::id)),
		// alias_column<als_c>(&Claim::id),
		alias_column<als_p>(&Patient::last_name),
		conc(conc(alias_column<als_d>(&Doctor::last_name), " "),alias_column<als_d>(&Doctor::first_name)),
		alias_column<als_c>(&Claim::status),
		alias_column<als_c>(&Claim::start_date),
		alias_column<als_c>(&Claim::submission_date),
		alias_column<als_c>(&Claim::amount),
		alias_column<als_c>(&Claim::asprose_claim_number),
		alias_column<als_m>(&Medication::name),
		// alias_column<als_i>(&Invoice::fkey_INSResponse),
		is_equal(alias_column<als_i>(&Invoice::fkey_INSResponse),1)),
		// sum(alias_column<als_i>(&Invoice::amount))),
		inner_join<als_p>(on(c(alias_column<als_p>(&Patient::id)) == alias_column<als_c>(&Claim::fkey_patient))),
		inner_join<als_d>(on(c(alias_column<als_d>(&Doctor::id)) == alias_column<als_c>(&Claim::fkey_doctor))),
		inner_join<als_m>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),
		// inner_join<als_c>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),
		inner_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))),
		where(t),
		order_by(alias_column<als_c>(&Claim::submission_date)).desc());
#endif
#else
	auto& storage = Storage::getStorage();
	
	// select c.id_claim, (SELECT count(*) > 0 from Invoices ii WHERE ii.fkey_claim = c.id_claim and ii.fkey_INSResponse = 1), i.fkey_INSResponse = 1, i.number from Invoices i INNER JOIN Claims c on i.fkey_claim = c.id_claim
	// auto otherlines = Storage::getStorage().select(columns(alias_column<als_c>(&Claim::id), greater_than(count<als_i>(), 0)),
	// 	inner_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))));

	// auto otherlines = storage.select(columns(alias_column<als_p>(&Invoice::id), greater_than(count<als_p>(), 0)),
	// 	inner_join<als_k>(on(c(alias_column<als_k>(&Claim::id)) == alias_column<als_p>(&Invoice::fkey_claim))),
	// 	where(c(alias_column<als_p>(&Invoice::fkey_INSResponse)) == 1));

	// auto otherlines = storage.select(columns(count<als_q>()),
	// 	inner_join<als_c>(on(c(alias_column<als_c>(&Claim::id)) == alias_column<als_q>(&Invoice::fkey_claim))),
	// 	where(c(alias_column<als_q>(&Invoice::fkey_INSResponse)) == 1));
/*

	SELECT
		c.*,
		IFNULL(i.response_1_count, 0)  AS response_1_count
		FROM
		Claims c
		LEFT JOIN
		(
			SELECT fkey_claim, COUNT(*) AS response_1_count
			FROM Invoices
			WHERE fkey_INSResponse = 1
			GROUP BY fkey_claim
		)
		AS i
		ON i.fkey_claim = c.id_claim
*/

/*
	select c.id_claim, (
		SELECT count(*) > 0
		from Invoices ii
		WHERE ii.fkey_claim = c.id_claim and ii.fkey_INSResponse = 1
		), i.fkey_INSResponse = 1, i.number
		from Invoices i
		INNER JOIN Claims c on i.fkey_claim = c.id_claim
	*/
	
	// auto otherlines = storage.select(columns(alias_column<als_c>(&Claim::id), as<response_1_count>(coalesce<int>(alias_column<als_c>(&Claim::other_system_id), 0))),
	// 	as<ii>(left_join<als_i>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)))));

	// auto otherlines = storage.select(columns(alias_column<als_c>(&Claim::id), as<response_1_count>(coalesce<int>(alias_column<als_c>(&Claim::other_system_id), 0))),
	// 	left_join<als_i>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))));
#if 1
	// auto lines = storage.select(columns( count<als_i>()),
	// 	where(c(alias_column<als_i>(&Invoice::fkey_INSResponse)) == 1 && c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)));

	// auto otherlines = storage.select(columns(alias_column<als_c>(&Claim::id),
	// 	select(columns(greater_than(count<als_i>(), 0)),
	// 			where(c(alias_column<als_i>(&Invoice::fkey_INSResponse)) == 1),
	// 			inner_join<als_c>(on(c(alias_column<als_c>(&Claim::id)) == alias_column<als_i>(&Invoice::fkey_claim)))),
	// 	inner_join<als_i>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)))));

	auto otherlines = storage.select(columns(alias_column<als_c>(&Claim::id),
		select(greater_than(count<als_q>(), 0), from<als_q>(),
			where(c(alias_column<als_q>(&Invoice::fkey_INSResponse)) == 1 && c(alias_column<als_q>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)))),
		from<als_i>(), inner_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))), group_by(alias_column<als_c>(&Claim::id)));

#elif 0
	auto otherlines = storage.select(columns(alias_column<als_c>(&Claim::id),
		select(columns(greater_than(count<als_q>(), 0)),
			where(c(alias_column<als_q>(&Invoice::fkey_INSResponse)) == 1 && c(alias_column<als_q>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)))),
		inner_join<als_i>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))));

#elif 0
	auto otherlines = storage.select(columns(alias_column<als_c>(&Claim::id),
		select(columns(greater_than(count<als_q>(), 0)),
			inner_join<als_l>(on(c(alias_column<als_l>(&Claim::id)) == alias_column<als_q>(&Invoice::fkey_claim))),
			where(c(alias_column<als_q>(&Invoice::fkey_INSResponse)) == 1))),
		inner_join<als_i>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))));
#elif 0
	auto otherlines = storage.select(columns(alias_column<als_c>(&Claim::id),
		select(columns(greater_than(count<als_p>(), 0)),
			inner_join<als_m>(on(c(alias_column<als_m>(&Claim::id)) == alias_column<als_p>(&Invoice::fkey_claim))),
			where(c(alias_column<als_p>(&Invoice::fkey_INSResponse)) == 1))),
		inner_join<als_i>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))));
#else
	auto otherlines = storage.select(columns(alias_column<als_c>(&Claim::id),
		select(columns(count<als_i>(),
			inner_join<als_c>(on(c(alias_column<als_c>(&Claim::id)) == alias_column<als_i>(&Invoice::fkey_claim)))),
			where(c(alias_column<als_i>(&Invoice::fkey_INSResponse)) == 0)),
		inner_join<als_i>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id));
#endif

	// auto x1 = std::get<0>(otherlines[0]);
	// auto x2 = std::get<1>(otherlines[0]);
	// auto x3 = std::get<0>(x2);
	// constexpr auto size = std::tuple_size_v<decltype(x2)>;

	// auto otherlines = storage.select(columns(alias_column<als_c>(&Claim::id), 
	// 	storage.select(columns(greater_than(count<als_q>(), 0)),
	// 		inner_join<als_l>(on(c(alias_column<als_l>(&Claim::id)) == alias_column<als_q>(&Invoice::fkey_claim))),
	// 		where(c(alias_column<als_q>(&Invoice::fkey_INSResponse)) == 1))),
	// 	inner_join<als_i>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))));

#endif

	long count = otherlines.size();
	auto strCount = Util::to_cstring(count);
	// m_countMainGrid.SetWindowTextW(strCount);

#ifndef  NESTED_SELECT	
	std::vector<std::string> headers{ "REENBOLSO", "PACIENTE", "DOCTOR", "SENT", "INICIAL", "ENTREGA", "MONTO REENBOLSO", "ASPROSE", "MEDICATION", "PENDING"};
#else
	std::vector<std::string> headers{ "REENBOLSO", "PACIENTE" };  //, "DOCTOR", "SENT", "INICIAL", "ENTREGA", "MONTO REENBOLSO", "ASPROSE", "MEDICATION", "RESP PENDING"};
#endif
	m_displayer_claims.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<7>, IntegerList<0>>(m_grid_1, std::move(otherlines), std::move(headers))); // , ColonesFormat<14>{13}, DolaresFormat<14>{14}));
	m_displayer_claims->display();


#endif
}
template <class T>
void MainView::InitializeGridINSResponses(const T& t)
{
#if 1
	

		auto otherlines = Storage::getStorage().select(columns(
			alias_column<als_c>(&Claim::id),
			alias_column<als_k>(&INSResponseLine::id),
			alias_column<als_k>(&INSResponseLine::fkey_factura),
			alias_column<als_k>(&INSResponseLine::fkey_INSResponse),
			alias_column<als_k>(&INSResponseLine::deducible_anual),
			alias_column<als_k>(&INSResponseLine::coaseguros),
			alias_column<als_k>(&INSResponseLine::copago),
			alias_column<als_k>(&INSResponseLine::monto_cubierto),
			alias_column<als_k>(&INSResponseLine::porcentaje_de_factura_cubierto),
			// alias_column<als_i>(&Invoice::id),
			alias_column<als_i>(&Invoice::number),
			alias_column<als_i>(&Invoice::amount),
			substr(alias_column<als_i>(&Invoice::description), 0, 50)),
			
			// alias_column<als_m>(&Medication::id)),
			// alias_column<als_c>(&Claim::amount)),
			left_join<als_i>(on(c(alias_column<als_k>(&INSResponseLine::fkey_factura)) == alias_column<als_i>(&Invoice::id))),
			// left_join<als_i>(on(c(alias_column<als_i>(&Invoice::id) == alias_column<als_k>(&INSResponseLine::fkey_factura))),
			inner_join<als_c>(on(c(alias_column<als_c>(&Claim::id)) == alias_column<als_i>(&Invoice::fkey_claim))),
			inner_join<als_m>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),
			// inner_join<als_i>(on(c(alias_column<als_k>(&INSResponseLine::fkey_INSResponse)) == alias_column<als_j>(&INSResponse::id))),
			where(t),
			// order_by(alias_column<als_c>(&Claim::start_date)).desc());
			order_by(alias_column<als_c>(&Claim::id)));

	
#else
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
		substr(alias_column<als_i>(&Invoice::description), 0, 50),
		alias_column<als_c>(&Claim::id),
		alias_column<als_k>(&INSResponseLine::fkey_INSResponse)),
		// alias_column<als_m>(&Medication::id)),
		// alias_column<als_c>(&Claim::amount)),
		left_join<als_i>(on(c(alias_column<als_k>(&INSResponseLine::fkey_factura)) == alias_column<als_i>(&Invoice::id))),
		// left_join<als_i>(on(c(alias_column<als_i>(&Invoice::id) == alias_column<als_k>(&INSResponseLine::fkey_factura))),
		inner_join<als_c>(on(c(alias_column<als_c>(&Claim::id)) == alias_column<als_i>(&Invoice::fkey_claim))),
		inner_join<als_m>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),
		// inner_join<als_i>(on(c(alias_column<als_k>(&INSResponseLine::fkey_INSResponse)) == alias_column<als_j>(&INSResponse::id))),
		where(t),
		// order_by(alias_column<als_c>(&Claim::start_date)).desc());
		order_by(alias_column<als_c>(&Claim::id)));

#endif
	
	
	
	std::vector<std::string> headers{ "REENBOLSO", "LINEA RES", "FACT", "RESPUESTA", "DEDUCIBLE", "COASEGURO", "COPAGO", "MONTO CUBIERTO", "% DE FACTURA", "FACT #", "FACT MONTO", "DESC" };
	m_displayer_responses.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<11>, IntegerList<5, 6, 7, 8>>(m_grid_2, std::move(otherlines), std::move(headers)));
	m_displayer_responses->display();
}



auto MainView::GetWhereStatement()
{
	auto num_factura = GetText(m_num_Factura);
	if (num_factura.empty())	num_factura = "%";
	auto factura_where = like(alias_column<als_i>(&Invoice::number), num_factura);

	auto desc_factura = GetText(m_invoice_description);
	if (desc_factura.empty())	desc_factura = "%";
	auto desc_where = like(alias_column<als_i>(&Invoice::description), desc_factura);
	
	auto claim = m_claimsCB.current();


	int claim_id = claim ? claim->id : -1;

	// auto claimWhere = (not claim.has_value() or (c(alias_column<als_c>(&Claim::id)) == claim->id));

	auto doctor = m_doctoresCB.current();
	int doctor_id = doctor ? doctor->id : -1;
	
	auto paciente = m_pacientesCB.current();
	int paciente_id = paciente ? paciente->id : -1;
	
	auto claim_amount_lower_bound = GetAmount(m_claim_amount);
	auto claim_amount_upper_bound = GetAmount(m_claim_amount_less_than);

	bool filter_dates = m_filter_by_dates.GetCheck();

	// TODO: Add your control notification handler code here
	auto start = GetDate(m_start_date);
	auto end = GetDate(m_end_date);

	auto whereStatement = (not filter_dates or (c(alias_column<als_c>(&Claim::start_date)) >= start
		&& c(alias_column<als_c>(&Claim::start_date)) <= end))
		&& (not doctor.has_value() or (c(alias_column<als_c>(&Claim::fkey_doctor)) == doctor_id))
		&& (not paciente.has_value() or (c(alias_column<als_c>(&Claim::fkey_patient)) == paciente_id))
		&& (not claim_amount_lower_bound > 0 or (c(alias_column<als_c>(&Claim::amount)) >= claim_amount_lower_bound))
		&& (not claim_amount_upper_bound > 0 or (c(alias_column<als_c>(&Claim::amount)) <= claim_amount_upper_bound))
		&& (not claim.has_value() or (c(alias_column<als_c>(&Claim::id)) == claim_id))
		&& factura_where
		&& desc_where;

	// auto whereStatement = (not filter_dates or (c(alias_column<als_c>(&Claim::start_date)) >= start
	// 	&& c(alias_column<als_c>(&Claim::start_date)) <= end))
	// 	&& (not doctor.has_value() or (c(alias_column<als_c>(&Claim::fkey_doctor)) == doctor->id))
	// 	&& (not paciente.has_value() or (c(alias_column<als_c>(&Claim::fkey_patient)) == paciente->id))
	// 	&& (not claim_amount_lower_bound > 0 or (c(alias_column<als_c>(&Claim::amount)) >= claim_amount_lower_bound))
	// 	&& (not claim.has_value() or (c(alias_column<als_c>(&Claim::id)) == claim->id))
	// 	&& factura_where;
	
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
	SetAmount(m_claim_amount_less_than, 0.0);
	SetText(m_num_Factura, "%"s);
	SetText(m_invoice_description, "%"s);
	OnBnClickedBFilter();
	// OnBnClickedBFilterInsresponses();
}



void MainView::OnBnClickedBRefresh()
{
	// TODO: Add your control notification handler code here
	Refresh();
}




void MainView::OnFilePrint()
{
	// TODO: Add your command handler code here
	m_grid_1.Print();
	m_grid_2.Print();
}

void MainView::OnGrid2StartSelChange(NMHDR* pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	auto row = pItem->iRow;
	auto col = pItem->iColumn;

	if (row < 1) return;

	auto invoice_id_cs = m_grid_2.GetItemText(row, 3);
	auto invoice_id_s = Util::to_string(invoice_id_cs.GetBuffer());
	auto invoice_id = std::stoi(invoice_id_s);

	InvoiceDlg dlg;
	dlg.m_id = invoice_id;
	dlg.DoModal();
	//Refresh();

}


void MainView::OnGrid1StartSelChange(NMHDR* pNotifyStruct, LRESULT*)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	auto row = pItem->iRow;
	auto col = pItem->iColumn;

	if (row < 1) return;
	
	auto claim_id_cs = m_grid_1.GetItemText(row, 1);
	auto claim_id_s = Util::to_string(claim_id_cs.GetBuffer());
	auto claim_id = std::stoi(claim_id_s);


	m_claimsCB.select(claim_id);

	auto whereStatement = (c(alias_column<als_c>(&Claim::id)) == claim_id);
	InitializeGridClaims(whereStatement);
	InitializeGridINSResponses(whereStatement);


	ClaimDlg dlg;
	dlg.m_id = claim_id;
	dlg.DoModal();
	//Refresh();
	//OnBnClickedBFilter();
}



void MainView::OnBnClickedBSelectReembolso()
{
	// TODO: Add your control notification handler code here

	auto selected =m_grid_2.GetSelectedCellRange();
	auto minrow = selected.GetMinRow();
	auto maxrow = selected.GetMaxRow();

	if (minrow < 0 || maxrow < 0)
		return;

	if (minrow != maxrow)
		return;
	
	auto text = m_grid_2.GetItemText(minrow, ID_REEMBOLSO_COLUMN);
	auto str = Util::from_cstring(text);
	auto id_reembolso = std::stoi(str);
	int i = 0;


	auto whereStatement = (c(alias_column<als_c>(&Claim::id)) == id_reembolso);
	InitializeGridClaims(whereStatement);
	InitializeGridINSResponses(whereStatement);

}


void MainView::OnBnClickedBCrearReembolso()
{
	// TODO: Add your control notification handler code here
	ClaimDlg dlg;
	dlg.DoModal();
	Refresh();
}
