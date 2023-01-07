// INSResponseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "INSResponseDlg.h"
#include "afxdialogex.h"
#include "INSResponseLineDlg.h"
#include "..\TesterForMFCUtilitiesDLL/JoinedGridDisplayer.h"

// INSResponseDlg dialog

IMPLEMENT_DYNAMIC(INSResponseDlg, CDialog)

INSResponseDlg::INSResponseDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_INSResponseDlg, pParent),
m_list_insresponsesLB(m_list_INSResponses, [](INSResponse& response)
{
		return Util::to_cstring(response.simple_dump());
})
{

}

INSResponseDlg::~INSResponseDlg()
{
}

void INSResponseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID_INSRESPONSE, m_id_insresponse);
	DDX_Control(pDX, IDC_E_LIQUIDACION_NUM, m_liquidacion);
	DDX_Control(pDX, IDC_E_NUMERO_CASO, m_no_caso);
	DDX_Control(pDX, IDC_E_TOTAL_BRUTO, m_total_bruto);
	DDX_Control(pDX, IDC_E_OTRAS_DEDUCCIONES, m_deducciones);
	DDX_Control(pDX, IDC_E_COPAGOS, m_copagos);
	DDX_Control(pDX, IDC_E_COASEGUROS, m_coaseguros);
	DDX_Control(pDX, IDC_E_DEDUCIBLE_ANUAL, m_deducible_anual);
	DDX_Control(pDX, IDC_E_TIPO_CAMBIO, m_tipo_cambio);
	DDX_Control(pDX, IDC_E_TOTAL_NETO, m_neto);
	DDX_Control(pDX, IDC_E_RETENCION, m_retencion);
	DDX_Control(pDX, IDC_E_TOTAL_A_PAGAR, m_total_pagar);
	DDX_Control(pDX, IDC_E_COMENTARIOS, m_comentarios);
	DDX_Control(pDX, IDC_L_INSRESPONSES_LIST, m_list_INSResponses);
	DDX_Control(pDX, IDC_GRID, m_grid);
	DDX_Control(pDX, IDC_D_DATE_RESPONSE, m_date_response);
	DDX_Control(pDX, IDC_E_SUM_INVOICES_AMOUNTS, m_sum_invoices_amount);
	DDX_Control(pDX, IDC_E_PORCENTAJE_PAGADO, m_porcentaje_pagado_INS_response);
	DDX_Control(pDX, IDC_E_SUM_PAGOS, m_sum_pagos);
}


BEGIN_MESSAGE_MAP(INSResponseDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &INSResponseDlg::OnBnClickedApply)
	ON_BN_CLICKED(ID_NUEVO, &INSResponseDlg::OnBnClickedNuevo)
	ON_BN_CLICKED(ID_BORRAR, &INSResponseDlg::OnBnClickedBorrar)
	ON_BN_CLICKED(ID_CERRAR, &INSResponseDlg::OnBnClickedCerrar)
	ON_BN_CLICKED(ID_CALCULATE, &INSResponseDlg::OnBnClickedCalculate)
	ON_LBN_SELCHANGE(IDC_L_INSRESPONSES_LIST, &INSResponseDlg::OnLbnSelchangeLInsresponsesList)
	ON_BN_CLICKED(ID_FILTER, &INSResponseDlg::OnBnClickedFilter)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID, OnGridStartSelChange)
END_MESSAGE_MAP()



void INSResponseDlg::OnGridStartSelChange(NMHDR* pNotifyStruct, LRESULT*)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	auto row = pItem->iRow;
	auto col = pItem->iColumn;

	if (row < 1) return;

	auto ins_response_line_id_cs = m_grid.GetItemText(row, 1);
	auto ins_response_line_id_s = Util::to_string(ins_response_line_id_cs.GetBuffer());
	auto ins_response_line_id = std::stoi(ins_response_line_id_s);

	INSResponseLineDlg dlg;
	dlg.m_id = ins_response_line_id;
	dlg.DoModal();
	Refresh();
	m_liquidacion.SetFocus();
	OnBnClickedFilter();
}



// INSResponseDlg message handlers


BOOL INSResponseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();

	if(m_id!= -1)
	{
		m_list_insresponsesLB.select(m_id);
	}
	else
	{
		this->OnBnClickedNuevo();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void INSResponseDlg::Refresh()
{
	m_list_insresponsesLB.loadLBOrderBy(&INSResponse::total_a_pagar);
}

void INSResponseDlg::OnBnClickedFilter()
{
	// TODO: Add your control notification handler code here
	auto whereClause = c(alias_column<als_k>(&INSResponseLine::fkey_INSResponse)) == m_ins_response->id;

	InitializeGrid(whereClause);
}

template <class T>
void INSResponseDlg::InitializeGrid(const T& t)
{
	// auto lines = Storage::getStorage().select(columns(
	// 	alias_column<als_i>(&Invoice::id),
	// 	alias_column<als_i>(&Invoice::number),
	// 	alias_column<als_i>(&Invoice::amount),
	// 	alias_column<als_i>(&Invoice::fkey_claim),
	// 	alias_column<als_c>(&Claim::start_date),
	// 	alias_column<als_c>(&Claim::submission_date),
	// 	alias_column<als_c>(&Claim::amount)),
	// 	// sum(alias_column<als_i>(&Invoice::amount))),
	// 	inner_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))),
	// 	where(t),
	// 	order_by(alias_column<als_i>(&Invoice::fkey_claim)));
	struct INSResponseLine_id : alias_tag {
		static const std::string& get() {
			static const std::string res = "INS Response Line";
			return res;
		}
	};
	auto statement = Storage::getStorage().prepare(select(columns(as<INSResponseLine_id>(&INSResponseLine::id),&INSResponseLine::fkey_factura)));
	//std::string colName{ statement.column_name(0) };
	////// end  testing column_name
	
	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_k>(&INSResponseLine::id),
		alias_column<als_k>(&INSResponseLine::fkey_factura),
		alias_column<als_i>(&Invoice::number),
		alias_column<als_k>(&INSResponseLine::monto_cubierto),
		alias_column<als_i>(&Invoice::amount),
		alias_column<als_k>(&INSResponseLine::porcentaje_de_factura_cubierto),
		alias_column<als_k>(&INSResponseLine::fkey_INSResponse)),
		inner_join<als_i>(on(c(alias_column<als_i>(&Invoice::id)) == alias_column<als_k>(&INSResponseLine::fkey_factura))),
		where(t),
		order_by(alias_column<als_k>(&INSResponseLine::monto_cubierto)));


	auto sum_results = Storage::getStorage().select(columns(
		sum(alias_column<als_i>(&Invoice::amount))),
		inner_join<als_i>(on(c(alias_column<als_i>(&Invoice::id)) == alias_column<als_k>(&INSResponseLine::fkey_factura))),
		where(t),
		group_by(alias_column<als_i>(&Invoice::fkey_INSResponse)));    // join 

	assert(sum_results.size() <= 1);
	auto sum_amounts = 0.0;
	if (!sum_results.empty())
	{
		auto&& line = sum_results[0];
		auto&& pc = std::get<0>(line);
		sum_amounts = *pc;
		SetAmount(m_sum_invoices_amount, sum_amounts);
	}

	auto sum_total_pagar = Storage::getStorage().select(columns(
		sum(alias_column<als_k>(&INSResponseLine::total_rubro_factura))),
		// inner_join<als_i>(on(c(alias_column<als_i>(&Invoice::id)) == alias_column<als_k>(&INSResponseLine::fkey_factura))),
		where(t),
		group_by(alias_column<als_k>(&INSResponseLine::fkey_INSResponse)));    // join

	
	if( !sum_total_pagar.empty())
	{
		auto&& line = sum_total_pagar[0];
		auto&& pc = std::get<0>(line);
		auto sum_total_pagar_amount = *pc;
		auto tipo_cambio = GetAmount(m_tipo_cambio);
		sum_total_pagar_amount *= tipo_cambio;
		SetAmount(m_sum_pagos, sum_total_pagar_amount );
		auto percent = sum_total_pagar_amount / sum_amounts * 100;
		SetAmount(m_porcentaje_pagado_INS_response, percent);
	}
	
	long count = otherlines.size();
	auto strCount = Util::to_cstring(count);
	// m_countMainGrid.SetWindowTextW(strCount);

	std::vector<std::string> headers{ "ID", "FAC ID",  "FAC NUM", "MONTO CUBIERTO", "FAC MONTO", "% FACT CUBIERTA", "INS RESPUESTA ID" };

	// m_grid.SetRowCount(0);
	// m_grid.SetColumnCount(0);
	m_displayer.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<5>, IntegerList<4>>(m_grid, std::move(otherlines), std::move(headers)));
	m_displayer->display();

}


void INSResponseDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	auto ins_response = getCurrent<INSResponse>(m_id_insresponse);

	std::chrono::sys_days date = GetDate(m_date_response );

	constexpr bool eq = sizeof(long) == sizeof(int);
	// date::sys_days submission_date = GetDate(m_date_submitted);

	auto liquidacion = GetText(m_liquidacion);
	auto num_caso = GetLongLong(m_no_caso);
	auto total_bruto = GetAmount(m_total_bruto);
	auto deducciones = GetAmount(m_deducciones);
	auto copagos = GetAmount(m_copagos);
	auto coaseguros = GetAmount(m_coaseguros);
	auto deducible_anual = GetAmount(m_deducible_anual);
	auto tipo_cambio = GetAmount(m_tipo_cambio);
	auto total_neto = GetDolares(m_neto).getAsLongDouble();
	auto retenciones = GetAmount(m_retencion);
	auto total_pagar = GetAmount(m_total_pagar);
	auto comentarios = GetText(m_comentarios);

	// calculados
	// auto monto_cubierto_p100 = 100 * monto_cubierto != 0 ? total_a_pagar / monto_cubierto : 0; //GetAmount(m_monto_cubierto_p100);
	// auto factura_cubierta_p100 = 100 * monto_factura != 0 ? total_a_pagar / monto_factura : 0;

	auto count = Storage::getStorage().count<INSResponse>(where(is_equal(&INSResponse::liquidacion_num, liquidacion)));

	if(tipo_cambio == 0)
	{
		MessageBoxW(L"No se puede dejar en 0 el tipo de cambio");
		return;
	}
	if (!ins_response)
	{
		if (count == 1)		// INSResponse already exists
		{
			return;	// cannot add another one
		}
	}


	if (!ins_response)	// insert
	{
		ins_response = m_list_insresponsesLB.insert(liquidacion, num_caso, total_bruto, deducciones, copagos, coaseguros, deducible_anual, total_neto, retenciones, total_pagar, comentarios, tipo_cambio, date);
		m_list_insresponsesLB.insert_into_listbox(*ins_response);
	}
	else                // update
	{
		ins_response->deducible_anual = deducible_anual;
		ins_response->comentarios = comentarios;
		ins_response->tipo_cambio = tipo_cambio;
		ins_response->total_neto = total_neto;
		ins_response->coaseguros = coaseguros;
		ins_response->copagos = copagos;
		ins_response->coaseguros = coaseguros;
		ins_response->date_response = date;
		ins_response->liquidacion_num = liquidacion;
		ins_response->numero_caso = num_caso;
		ins_response->otras_deducciones = deducciones;
		ins_response->retencion = retenciones;
		ins_response->total_a_pagar = total_pagar;
		ins_response->total_bruto = total_bruto;		
		m_list_insresponsesLB.update(*ins_response);
	}
	m_list_insresponsesLB.loadLBOrderBy(&INSResponse::date_response);
	m_ins_response = ins_response;
	setIdFromRecord<INSResponse>(m_id_insresponse, ins_response->id);
}


void INSResponseDlg::OnBnClickedNuevo()
{
	// TODO: Add your control notification handler code here
	SetText(m_id_insresponse, ""s);

	// date::sys_days date = GetDate(m_date_response);

	// date::sys_days submission_date = GetDate(m_date_submitted);

	SetText(m_liquidacion, ""s);
	SetLongLong(m_no_caso, 0LL);
	SetAmount(m_total_bruto, 0.0);
	SetAmount(m_deducciones, 0.0);
	SetAmount(m_copagos, 0.0);
	SetAmount(m_coaseguros, 0.0);
	SetAmount(m_deducible_anual, 0.0);
	SetAmount(m_tipo_cambio, 0.0);
	SetDolares(m_neto, 0.0);
	SetAmount(m_retencion, 0.0);
	SetAmount(m_total_pagar, 0.0);
	SetText(m_comentarios, ""s);

	// calculados
	// auto monto_cubierto_p100 = 100 * monto_cubierto != 0 ? total_a_pagar / monto_cubierto : 0; //GetAmount(m_monto_cubierto_p100);
	// auto factura_cubierta_p100 = 100 * monto_factura != 0 ? total_a_pagar / monto_factura : 0;

	m_list_insresponsesLB.loadLBOrderBy(&INSResponse::date_response);
	m_list_insresponsesLB.select(-1);
	m_ins_response = std::nullopt;
}


void INSResponseDlg::OnBnClickedBorrar()
{
	// TODO: Add your control notification handler code here
	if (m_list_insresponsesLB.delete_current_sel())
	{
		OnBnClickedNuevo();
	}
}


void INSResponseDlg::OnBnClickedCerrar()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void INSResponseDlg::OnBnClickedCalculate()
{
	// TODO: Add your control notification handler code here
	auto total_bruto = GetAmount(m_total_bruto);
	auto deducciones = GetAmount(m_deducciones);
	auto copagos = GetAmount(m_copagos);
	auto coaseguros = GetAmount(m_coaseguros);
	auto deducible_anual = GetAmount(m_deducible_anual);
	auto total_neto_en_dolares = total_bruto - deducciones - copagos - coaseguros - deducible_anual;

	auto tipo_cambio = GetAmount(m_tipo_cambio);
	auto total_neto_en_colones = total_neto_en_dolares * tipo_cambio;

//	SetAmount(m_neto, total_neto_en_dolares);
	SetDolares(m_neto, total_neto_en_dolares);
	auto retencion = GetAmount(m_retencion);
	auto total_a_pagar = total_neto_en_colones - retencion;
	SetAmount(m_total_pagar, total_a_pagar);
	auto suma_facturas = GetAmount(m_sum_invoices_amount);
	if( suma_facturas == 0)
	{
		MessageBoxW(L"Suma facturas esta vacio");
		return;
	}
	auto porcentaje_pagado = total_a_pagar / suma_facturas * 100;
	SetAmount(m_porcentaje_pagado_INS_response, porcentaje_pagado);
}


void INSResponseDlg::OnLbnSelchangeLInsresponsesList()
{
	// TODO: Add your control notification handler code here
	m_ins_response = m_list_insresponsesLB.current();
	if (!m_ins_response)  return;

	SetAmount(m_id_insresponse, m_ins_response->id);
	SetText(m_liquidacion, m_ins_response->liquidacion_num);
	SetAmount(m_no_caso, m_ins_response->numero_caso);
	SetAmount(m_total_bruto, m_ins_response->total_bruto);
	SetAmount(m_deducciones, m_ins_response->otras_deducciones);
	SetAmount(m_copagos, m_ins_response->copagos);
	SetAmount(m_coaseguros, m_ins_response->coaseguros);
	SetAmount(m_deducible_anual, m_ins_response->deducible_anual);
	SetAmount(m_tipo_cambio, m_ins_response->tipo_cambio);
	SetDolares(m_neto, m_ins_response->total_neto);
	SetAmount(m_retencion, m_ins_response->retencion);
	SetAmount(m_total_pagar, m_ins_response->total_a_pagar);
	SetText(m_comentarios, m_ins_response->comentarios);
	SetDate(m_date_response, m_ins_response->date_response);

	OnBnClickedFilter();
}


