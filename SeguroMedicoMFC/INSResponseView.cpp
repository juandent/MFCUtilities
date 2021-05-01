// INSResponseView.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "INSResponseView.h"
#include "INSResponseDoc.h"
#include "Data.h"
#include "INSResponseDlg.h"

#include "JoinedGridDisplayer.h"


// INSResponseView

IMPLEMENT_DYNCREATE(INSResponseView, CFormView)

INSResponseView::INSResponseView()
	: CFormView(IDD_INSResponseView),
m_insresponseCB(m_lista_ins_response, [](INSResponse& resp)
{
		return Util::to_cstring(resp.simple_dump());
}),
m_doctoresCB(m_lista_doctores, [](Doctor& doctor)
{
		return Util::to_cstring(doctor.simple_dump());
}),
m_patientsCB(m_lista_pacientes, [](Patient& patient)
{
		return Util::to_cstring(patient.simple_dump());
}),
m_medicationsCB(m_lista_medicamentos, [](Medication& med)
{
		return Util::to_cstring(med.simple_dump());
})
{
	
}

INSResponseView::~INSResponseView()
{
}

void INSResponseView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_VIEW_1, m_grid_1);
	DDX_Control(pDX, IDC_GRID_VIEW_2, m_grid_2);
	DDX_Control(pDX, IDC_C_INS_RESPONSE_IDS, m_lista_ins_response);
	DDX_Control(pDX, IDC_C_LISTA_DOCTORES, m_lista_doctores);
	DDX_Control(pDX, IDC_C_LISTA_PACIENTES, m_lista_pacientes);
	DDX_Control(pDX, IDC_C_LISTA_MEDICAMENTOS, m_lista_medicamentos);
	DDX_Control(pDX, IDC_FECHA_INICIO, m_fecha_inicio);
	DDX_Control(pDX, IDC_FECHA_FINAL, m_fecha_final);
	DDX_Control(pDX, IDC_C_FILTER_BY_DATES, m_filter_by_dates);
	DDX_Control(pDX, IDC_E_INSREPONSE_AMOUNT, m_insresponse_amount);
	DDX_Control(pDX, IDC_E_NUM_FACTURA2, m_num_factura);
}

BEGIN_MESSAGE_MAP(INSResponseView, CFormView)
	ON_BN_CLICKED(IDC_B_FILTER, &INSResponseView::OnBnClickedBFilter)
	ON_BN_CLICKED(IDC_B_CLEAR_FILTERS, &INSResponseView::OnBnClickedBClearFilters)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_VIEW_1, OnGrid1StartSelChange)
END_MESSAGE_MAP()


// INSResponseView diagnostics

#ifdef _DEBUG
void INSResponseView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void INSResponseView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


INSResponseDoc* INSResponseView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(INSResponseDoc)));
	return (INSResponseDoc*)m_pDocument;

}


// INSResponseView message handlers


void INSResponseView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	Refresh();

	
	// TODO: Add your specialized code here and/or call the base class
	// InitializeGridINSResponse(true);
	// InitializeGridINSResponseSummary(true);
	OnBnClickedBFilter();
}

void INSResponseView::Refresh()
{
	m_insresponseCB.loadLBOrderByDesc(&INSResponse::date_response);
	m_doctoresCB.loadLBOrderBy(&Doctor::last_name);
	m_patientsCB.loadLBOrderBy(&Patient::last_name);
	m_medicationsCB.loadLBOrderBy(&Medication::name);
}


#if 0
using als_c = alias_c<Claim>;
using als_p = alias_p<Patient>;
using als_d = alias_d<Doctor>;
using als_s = alias_s<Specialty>;
using als_m = alias_m<Medication>;
using als_i = alias_i<Invoice>;
using als_j = alias_j<INSResponse>;
using als_k = alias_k<INSResponseLine>;

#endif

template<typename T>
void INSResponseView::InitializeGridINSResponse(const T& t)
{

	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_j>(&INSResponse::id),
		alias_column<als_j>(&INSResponse::date_response),
		alias_column<als_k>(&INSResponseLine::id),
		alias_column<als_k>(&INSResponseLine::monto_cubierto),
		alias_column<als_k>(&INSResponseLine::porcentaje_de_monto_cubierto),
		alias_column<als_k>(&INSResponseLine::porcentaje_de_factura_cubierto),
		alias_column<als_i>(&Invoice::number),
		alias_column<als_i>(&Invoice::amount)),

		// sum(alias_column<als_i>(&Invoice::amount))),
		inner_join<als_k>(on(c(alias_column<als_k>(&INSResponseLine::fkey_INSResponse)) == alias_column<als_j>(&INSResponse::id))),
		inner_join<als_i>(on(c(alias_column<als_k>(&INSResponseLine::fkey_factura)) == alias_column<als_i>(&Invoice::id))),
		inner_join<als_c>( on(c(alias_column<als_c>(&Claim::id)) == alias_column<als_i>(&Invoice::fkey_claim))),
		inner_join<als_d>(on(c(alias_column<als_c>(&Claim::fkey_doctor)) == alias_column<als_d>(&Doctor::id))),
		inner_join<als_p>(on(c(alias_column<als_p>(&Patient::id)) == alias_column<als_c>(&Claim::fkey_patient))),
		inner_join<als_m>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),

		where(t),
		// group_by(&INSResponse::id));
		order_by(alias_column<als_j>(&INSResponse::date_response)).desc());



	long count = otherlines.size();
	auto strCount = Util::to_cstring(count);
	// m_countMainGrid.SetWindowTextW(strCount);

	std::vector<std::string> headers{ "ID RESPUESTA", "FECHA RESP", "ID LINEA RESP", "MONTO CUBIERTO", "% MONTO CUBIERTO", "% FACTURA", "NUM FACT", "MONTO FACT" };

	m_displayer_ins_response.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<8>, IntegerList<4>>(m_grid_1, std::move(otherlines), std::move(headers)));
	m_displayer_ins_response->display();

}
#if 0
static double extractValue(std::unique_ptr<double>& val)
{
	if (val)
	{
		return *val;
	}
	return 0.0;
}
template<typename T> requires !std::is_same_v<T, std::unique_ptr<double>&>
	static T extractValue(const T& t)
	{
		return t;
	}
#endif

struct pagado : alias_tag {
	static const std::string& get() {
		static const std::string res = "pagado";
		return res;
	}
};




template<typename T>
void INSResponseView::InitializeGridINSResponseSummary(const T& t)
{

	auto otherlines = Storage::getStorage().select(columns(
		alias_column< als_j>(&INSResponse::id),
		// alias_column<als_i>(&Invoice::description),

		sum(alias_column<als_i>(&Invoice::amount)),
		// div(sum(mul(alias_column<als_k>(&INSResponseLine::total_rubro_factura), alias_column<als_j>(&INSResponse::tipo_cambio))), sum(alias_column<als_i>(&Invoice::amount))),
		sum( mul(alias_column<als_k>(&INSResponseLine::total_rubro_factura), alias_column<als_j>(&INSResponse::tipo_cambio)))),
		// alias_column<als_k>(&INSResponseLine::id)),

		inner_join<als_k>(on(c(alias_column<als_k>(&INSResponseLine::fkey_INSResponse)) == alias_column<als_j>(&INSResponse::id))),
		inner_join<als_i>(on(c(alias_column<als_k>(&INSResponseLine::fkey_factura)) == alias_column<als_i>(&Invoice::id))),
		// inner_join<als_c>(on(c(alias_column<als_c>(&Claim::id)) == alias_column<als_i>(&Invoice::fkey_claim))),
		// inner_join<als_d>(on(c(alias_column<als_c>(&Claim::fkey_doctor)) == alias_column<als_d>(&Doctor::id))),
		// inner_join<als_p>(on(c(alias_column<als_p>(&Patient::id)) == alias_column<als_c>(&Claim::fkey_patient))),
		// inner_join<als_m>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),

		where(true),
		group_by(alias_column < als_j>(&INSResponse::id))); // ,
		// order_by(alias_column<als_j>(&INSResponse::date_response)).desc());



	long count = otherlines.size();
	if(count>0)
	{
#if 0
		auto line = std::move(otherlines[0]);
		auto&& val = std::get<2>(line);
		using ValueType = decltype(std::get<2>(line));
		static_assert(std::is_same_v<ValueType, std::unique_ptr<double>&>);

		double v;
		
		if constexpr (std::is_same_v<ValueType, std::unique_ptr<double>&>)
		{
			v = *val;
		}
		
		// auto& ve = extractValue(std::move(std::get<2>(line)));
		// CString str = Util::to_cstring(ve);
		auto& desc = extractValue(std::get<1>(line));
		CString str = Util::to_cstring(desc);
#if 0
		// if constexpr (std::is_same_v<std::unique_ptr<double>&, ValueType>)
		static_assert(std::is_same_v<std::unique_ptr<double>&, ValueType>);
		auto x = Util::to_cstring(std::move(val));
		// auto x = Util::unique_ptr_to_cstring(std::move(val));

		auto&& v0 = std::get<1>(line);
		auto xx= Util::to_cstring(std::move(v0));

		int i = 0;
#endif
#elif 0	
		//CString cs = Util::to_cstring( val);

		auto line = std::move(otherlines[0]);
		const auto& val = std::get<1>(line);
		using ValueType = decltype(val);
		ValueType* p;
		static_assert(std::is_same_v<std::unique_ptr<double>, ValueType>);
		auto x = Util::unique_ptr_to_cstring(std::move(val));
		
#endif	
	}
	auto strCount = Util::to_cstring(count);
	// m_countMainGrid.SetWindowTextW(strCount);

	std::vector<std::string> headers{ "INS RESP ID", "SUM INV AMT", "SUM INV PAID" };

#if 1
	m_displayer_ins_response_summary.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<2,3>, IntegerList<0>>(m_grid_2, std::move(otherlines), std::move(headers)));
	m_displayer_ins_response_summary->display();
#endif
}


auto INSResponseView::GetWhereStatement()
{
	auto response = m_insresponseCB.current();

	auto response_id = response ? response->id : -1;
	auto responseWhere = (not response or (c(alias_column<als_j>(&INSResponse::id)) == response_id));

	auto doctor = m_doctoresCB.current();
	auto doctor_id = doctor ? doctor->id : -1;
	
	auto doctorWhere = (not doctor or (c(alias_column<als_d>(&Doctor::id)) == doctor_id));

	auto patient = m_patientsCB.current();
	auto patient_id = patient ? patient->id : -1;

	
	auto patientWhere = (not patient or (c(alias_column<als_p>(&Patient::id)) == patient_id));

	auto medication = m_medicationsCB.current();
	auto medication_id = medication ? medication->id : -1;

	auto medicationWhere = (not medication or (c(alias_column<als_m>(&Medication::id)) == medication_id));

	auto filter_by_dates = m_filter_by_dates.GetCheck();

	auto start_date = GetDate(m_fecha_inicio);
	auto finish_date = GetDate(m_fecha_final);

	auto filter_by_dates_where = (not filter_by_dates or
		(c(alias_column<als_j>(&INSResponse::date_response)) >= start_date
			&& (c(alias_column<als_j>(&INSResponse::date_response)) <= finish_date)));

	auto amount = GetAmount(m_insresponse_amount);

	auto amount_where = (not amount or (c(alias_column<als_j>(&INSResponse::total_a_pagar))));

	auto num_factura = GetText(m_num_factura);

	num_factura = num_factura.empty() ? "%" : num_factura;

	auto num_factura_where = (like (alias_column<als_i>(&Invoice::number) ,num_factura));
	
	return responseWhere && doctorWhere && patientWhere && medicationWhere && filter_by_dates_where && amount_where && num_factura_where;
}

void INSResponseView::OnBnClickedBFilter()
{
	// TODO: Add your control notification handler code here
	auto whereStatement = GetWhereStatement();
	InitializeGridINSResponse(whereStatement);
	InitializeGridINSResponseSummary(whereStatement);
	
}


void INSResponseView::OnBnClickedBClearFilters()
{
	// TODO: Add your control notification handler code here
	m_medicationsCB.select(std::nullopt);
	m_doctoresCB.select(std::nullopt);
	m_insresponseCB.select(std::nullopt);
	m_patientsCB.select(std::nullopt);
	m_filter_by_dates.SetCheck(0);
	SetAmount(m_insresponse_amount, 0.0);
	SetText(m_num_factura, "%"s);
	OnBnClickedBFilter();
}


void INSResponseView::OnGrid1StartSelChange(NMHDR* pNotifyStruct, LRESULT*)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	auto row = pItem->iRow;
	auto col = pItem->iColumn;

	if (row < 1) return;

	auto response_id_cs = m_grid_1.GetItemText(row, 1);
	auto response_id_s = Util::to_string(response_id_cs.GetBuffer());
	auto response_id = std::stoi(response_id_s);

	INSResponseDlg dlg;
	dlg.m_id = response_id;
	dlg.DoModal();
	Refresh();
	OnBnClickedBFilter();
}
