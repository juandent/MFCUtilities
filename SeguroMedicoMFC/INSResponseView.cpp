// INSResponseView.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "INSResponseView.h"
#include "INSResponseDoc.h"
#include "Data.h"

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
}

BEGIN_MESSAGE_MAP(INSResponseView, CFormView)
	ON_BN_CLICKED(IDC_B_FILTER, &INSResponseView::OnBnClickedBFilter)
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
	InitializeGridINSResponse(true);
	InitializeGridINSResponseSummary(true);
}

void INSResponseView::Refresh()
{
	m_insresponseCB.loadLBOrderByDesc(&INSResponse::date_response);
	m_doctoresCB.loadLBOrderBy(&Doctor::last_name);
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

		where(t),
		// group_by(&INSResponse::id));
		order_by(alias_column<als_j>(&INSResponse::date_response)).desc());



	long count = otherlines.size();
	auto strCount = Util::to_cstring(count);
	// m_countMainGrid.SetWindowTextW(strCount);

	std::vector<std::string> headers{ "ID RESPUESTA", "ID LINEA RESP", "MONTO CUBIERTO", "% MONTO CUBIERTO", "% FACTURA", "NUM FACT", "MONTO FACT" };

	m_displayer_ins_response.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<3,7>, IntegerList<0>>(m_grid_1, std::move(otherlines), std::move(headers)));
	m_displayer_ins_response->display();

}

template<typename T>
void INSResponseView::InitializeGridINSResponseSummary(const T& t)
{

	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_i>(&Invoice::id),
		alias_column<als_i>(&Invoice::description)),

		// sum(alias_column<als_i>(&Invoice::amount))),
		inner_join<als_k>(on(c(alias_column<als_k>(&INSResponseLine::fkey_INSResponse)) == alias_column<als_j>(&INSResponse::id))),
		inner_join<als_i>(on(c(alias_column<als_k>(&INSResponseLine::fkey_factura)) == alias_column<als_i>(&Invoice::id))),

		where(t),
		// group_by(&INSResponse::id));
		order_by(alias_column<als_j>(&INSResponse::date_response)).desc());



	long count = otherlines.size();
	if(count>0)
	{
#if 0
		auto line = std::move(otherlines[0]);
		auto&& val = std::get<2>(line);
		using ValueType = decltype(val);


		// if constexpr (std::is_same_v<std::unique_ptr<double>&, ValueType>)
		static_assert(std::is_same_v<std::unique_ptr<double>&, ValueType>);
		auto x = Util::to_cstring(std::move(val));
		// auto x = Util::unique_ptr_to_cstring(std::move(val));

		auto&& v0 = std::get<1>(line);
		auto xx= Util::to_cstring(std::move(v0));

		int i = 0;
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

	std::vector<std::string> headers{ "ID FACT", "DESC" };

#if 1
	m_displayer_ins_response_summary.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<3, 7>, IntegerList<0>>(m_grid_2, std::move(otherlines), std::move(headers)));
	m_displayer_ins_response_summary->display();
#endif
}


auto INSResponseView::GetWhereStatement()
{
	auto response = m_insresponseCB.current();
	
	auto responseWhere = (not response or (c(alias_column<als_j>(&INSResponse::id)) == response->id));

	auto doctor = m_doctoresCB.current();

	auto doctorWhere = (not doctor or (c(alias_column<als_d>(&Doctor::id)) == doctor->id));
	
	return responseWhere && doctorWhere;
}

void INSResponseView::OnBnClickedBFilter()
{
	// TODO: Add your control notification handler code here
	auto whereStatement = GetWhereStatement();
	InitializeGridINSResponse(whereStatement);
	InitializeGridINSResponseSummary(whereStatement);
	
}
