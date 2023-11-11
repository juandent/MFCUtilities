// MainView.cpp : implementation file
//

#include "pch.h"
#include "SeguroMedico.h"
#include "MainView.h"
#include "Data.h"


using namespace sqlite_orm;
import joinedgrid;

// MainView

IMPLEMENT_DYNCREATE(MainView, CFormView)

MainView::MainView()
	: CFormView(IDD_MAINVIEW)
{

}

MainView::~MainView()
{
}

void MainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_CLAIM, m_grid_claim);
	DDX_Control(pDX, IDC_GRID_FACTURAS, m_grid_facturas);
	DDX_Control(pDX, IDC_GRID_PENDING, m_grid_pending);
}

BEGIN_MESSAGE_MAP(MainView, CFormView)
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
	InitializeGridClaims(true);
	InitializeGridINSResponses(true);
	InitializeGridPendingInvoices(true);
}


template<typename T>
void MainView::InitializeGridClaims(const T& t)
{
	auto expression = select(columns(
		// distinct(alias_column<als_c>(&Claim::id)),
		alias_column<als_c>(&Claim::id),
		alias_column<als_p>(&Patient::last_name),
		conc(conc(alias_column<als_d>(&Doctor::last_name), " "), alias_column<als_d>(&Doctor::first_name)),
		alias_column<als_c>(&Claim::status),
		alias_column<als_c>(&Claim::start_date),
		alias_column<als_c>(&Claim::submission_date),
		alias_column<als_c>(&Claim::amount),
		alias_column<als_c>(&Claim::asprose_case_number),
		alias_column<als_m>(&Medication::name),
		// alias_column<als_i>(&Invoice::fkey_INSResponse),
		select(greater_than(count<als_q>(), 0), from<als_q>(),
			where(c(alias_column<als_q>(&Invoice::fkey_INSResponse)) == 1 && c(alias_column<als_q>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)))),
		from<als_i>(),
		// sum(alias_column<als_i>(&Invoice::amount))),
		left_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))),
		left_join<als_p>(on(c(alias_column<als_p>(&Patient::id)) == alias_column<als_c>(&Claim::fkey_patient))),
		left_join<als_d>(on(c(alias_column<als_d>(&Doctor::id)) == alias_column<als_c>(&Claim::fkey_doctor))),
		left_join<als_m>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),
		// inner_join<als_c>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),

		where(t),
		group_by(alias_column<als_c>(&Claim::id)),
		order_by(alias_column<als_c>(&Claim::id)).desc());

	auto sql = Storage::getStorage().dump(expression);
	auto statement = Storage::getStorage().prepare(expression);
	auto rows = Storage::getStorage().execute(statement);
	auto otherlines = rows;

	long count = otherlines.size();
	auto strCount = util::to_cstring(count);

	std::vector<std::string> headers{ "REENBOLSO", "PACIENTE", "DOCTOR", "SENT", "INICIAL", "ENTREGA", "MONTO REENBOLSO", "ASPROSE", "MEDICATION", "PENDING" };

	m_displayer_claims.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<7>, IntegerList<0>>(m_grid_claim, std::move(otherlines), std::move(headers))); // , ColonesFormat<14>{13}, DolaresFormat<14>{14}));
	m_displayer_claims->display();
}

template <class T>
void MainView::InitializeGridINSResponses(const T& t)
{
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
		order_by(alias_column<als_c>(&Claim::id)).desc());



	std::vector<std::string> headers{ "REENBOLSO", "LINEA RES", "FACT", "RESPUESTA", "DEDUCIBLE", "COASEGURO", "COPAGO", "MONTO CUBIERTO", "% DE FACTURA", "FACT #", "FACT MONTO", "DESC" };
	m_displayer_responses.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<11>, IntegerList<5, 6, 7, 8>>(m_grid_facturas, std::move(otherlines), std::move(headers)));
	m_displayer_responses->display();
}

template <class T>
void MainView::InitializeGridPendingInvoices(const T& t)
{
	int id;
	int fkey_claim;
	std::string number;
	double amount;
	int type;
	std::string description;
	std::optional<int> fkey_INSResponse;

	// auto where_clause = (is_null(alias_column<als_i>(&Invoice::fkey_INSResponse)));
	auto where_clause = t and c(alias_column<als_i>(&Invoice::fkey_INSResponse)) == 1;



	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_c>(&Claim::id),
		alias_column<als_c>(&Claim::amount),
		alias_column<als_c>(&Claim::start_date),
		alias_column<als_i>(&Invoice::id),
		alias_column<als_i>(&Invoice::number),
		alias_column<als_i>(&Invoice::amount),
		alias_column<als_i>(&Invoice::type),
		substr(alias_column<als_i>(&Invoice::description), 0, 100),
		alias_column<als_i>(&Invoice::fkey_INSResponse)),

		// alias_column<als_m>(&Medication::id)),

		inner_join<als_c>(on(c(alias_column<als_c>(&Claim::id)) == alias_column<als_i>(&Invoice::fkey_claim))),
		// inner_join<als_m>(on(c(alias_column<als_c>(&Claim::fkey_medication)) == alias_column<als_m>(&Medication::id))),
		// inner_join<als_i>(on(c(alias_column<als_k>(&INSResponseLine::fkey_INSResponse)) == alias_column<als_j>(&INSResponse::id))),
		where(where_clause),
		// order_by(alias_column<als_c>(&Claim::start_date)).desc());
		multi_order_by(order_by(alias_column<als_c>(&Claim::id)).desc(), order_by(alias_column<als_i>(&Invoice::number))));
	// order_by(alias_column<als_c>(&Claim::id)));

	std::vector<std::string> headers{ "REENBOLSO ID", "MONTO REEMBOLSO", "FECHA REEMBOLSO", "FACT ID", "FACT NUM", "FACT MONTO", "FACT TIPO", "FACT DESCR", "INS RESP ID" };
	m_displayer_pending_invoices.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<2, 6>, IntegerList<0>>(m_grid_pending, std::move(otherlines), std::move(headers)));
	m_displayer_pending_invoices->display();

}
