// INSResponseLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "INSResponseLineDlg.h"
#include "afxdialogex.h"
#include "INSResponseDlg.h"
#include "InvoiceDlg.h"


// INSResponseLineDlg dialog

IMPLEMENT_DYNAMIC(INSResponseLineDlg, CDialog)

INSResponseLineDlg::INSResponseLineDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_m_INSResponseLineDlg, pParent),
m_INSResponseLinesLB(m_list_INSResponseLines, [](INSResponseLine& line)
{
		return util::to_cstring(line.simple_dump());
}),
m_ins_responseCB(m_list_ins_response, [](INSResponse& response)
{
		return util::to_cstring(response.simple_dump());
}),
m_invoicesCB(m_list_facturas, [](Invoice& factura)
{
		return util::to_cstring(factura.simple_dump());
})
{

}

INSResponseLineDlg::~INSResponseLineDlg()
{
}

void INSResponseLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_C_FKEY_INSRESPONSE, m_list_ins_response);
	DDX_Control(pDX, IDC_C_FKEY_INVOICE, m_list_facturas);
	DDX_Control(pDX, IDC_E_MONTO_CUBIERTO, m_monto_cubierto);
	DDX_Control(pDX, IDC_E_MONTO_DEDUCCIONES, m_deducciones);
	DDX_Control(pDX, IDC_E_MONTO_COPAGOS, m_copago);
	DDX_Control(pDX, IDC_E_MONTO_COASEGUROS, m_coaseguros);
	DDX_Control(pDX, IDC_E_MONTO_DEDUCIBLE_ANUAL, m_deducible_anual);
	DDX_Control(pDX, IDC_E_MONTO_A_PAGAR, m_total_pagar);
	DDX_Control(pDX, IDC_E_MONTO_FACTURA, m_monto_factura);
	DDX_Control(pDX, IDC_E_MONTO_CUBIERTO_P100, m_monto_cubierto_p100);
	DDX_Control(pDX, IDC_E_FACTURA_CUBIERTO_P100, m_factura_cubierta_p100);
	DDX_Control(pDX, IDC_L_INSRESPONSELINE, m_list_INSResponseLines);
	DDX_Control(pDX, IDC_E_ID, m_id_response_line);
	DDX_Control(pDX, IDC_E_TIPO_CAMBIO, m_tipo_cambio);
	DDX_Control(pDX, IDC_E_MONTO_A_PAGAR_COLONES, m_total_pagar_colones);
	DDX_Control(pDX, IDC_E_FACTURA_NUM, m_factura_num);
}


BEGIN_MESSAGE_MAP(INSResponseLineDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &INSResponseLineDlg::OnBnClickedApply)
	ON_BN_CLICKED(ID_NUEVO, &INSResponseLineDlg::OnBnClickedNuevo)
	ON_BN_CLICKED(ID_BORRAR, &INSResponseLineDlg::OnBnClickedBorrar)
	ON_BN_CLICKED(ID_CERRAR, &INSResponseLineDlg::OnBnClickedCerrar)
	ON_CBN_SELCHANGE(IDC_C_FKEY_INSRESPONSE, &INSResponseLineDlg::OnCbnSelchangeCFkeyInsresponse)
	ON_CBN_SELCHANGE(IDC_C_FKEY_INVOICE, &INSResponseLineDlg::OnCbnSelchangeCFkeyInvoice)
	ON_EN_CHANGE(IDC_E_MONTO_A_PAGAR, &INSResponseLineDlg::OnEnChangeEMontoAPagar)
	ON_LBN_SELCHANGE(IDC_L_INSRESPONSELINE, &INSResponseLineDlg::OnLbnSelchangeLInsresponseline)
	ON_BN_CLICKED(ID_CALCULATE, &INSResponseLineDlg::OnBnClickedCalculate)
	ON_BN_CLICKED(IDC_B_INS_RESPONSES, &INSResponseLineDlg::OnBnClickedBInsResponses)
	ON_BN_CLICKED(IDC_B_INVOICES, &INSResponseLineDlg::OnBnClickedBInvoices)
	ON_BN_CLICKED(IDC_B_BRING_FROM_INSRESPONSE, &INSResponseLineDlg::OnBnClickedBBringFromInsresponse)
END_MESSAGE_MAP()


// INSResponseLineDlg message handlers


BOOL INSResponseLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();

	if(m_id!=-1)
	{
		m_INSResponseLinesLB.select(m_id);
	}
	if(! m_factura_numero.empty())
	{
		SetText(m_factura_num, m_factura_numero);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void INSResponseLineDlg::Refresh()
{
	m_INSResponseLinesLB.loadLBOrderByDesc(&INSResponseLine::id);
	m_ins_responseCB.loadLBOrderByDesc(&INSResponse::id);
	m_invoicesCB.loadLBOrderBy(&Invoice::amount);
}




void INSResponseLineDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	auto ins_response_line = getCurrent<INSResponseLine>(m_id_response_line);

	auto ins_response = m_ins_responseCB.current();
	auto factura = m_invoicesCB.current();


	// date::sys_days start_date = GetDate(m_start_date);
	// date::sys_days submission_date = GetDate(m_date_submitted);
	// TODO: calcular
	auto monto_cubierto = GetAmount(m_monto_cubierto);
	auto deducciones = GetAmount(m_deducciones);
	auto copago = GetAmount(m_copago);
	auto coaseguros = GetAmount(m_coaseguros);
	auto deducible_anual = GetAmount(m_deducible_anual);
	auto total_a_pagar = GetAmount(m_total_pagar);
	// ask factura:
	auto monto_factura = GetAmount(m_monto_factura);

	// calculados
	auto monto_cubierto_p100 = GetAmount(m_monto_cubierto_p100);
	auto factura_cubierta_p100 = GetAmount(m_factura_cubierta_p100); // 100 * monto_factura != 0 ? total_a_pagar / monto_factura : 0;

	if (!ins_response)
	{
		MessageBoxW(L"Falta escoger INS Response");
		return;
	}
	if (!factura)
	{
		MessageBoxW(L"Falta escoger factura");
		return;
	}
#if	1
	//
	auto count = Storage::getStorage().count<INSResponseLine>(where(is_equal(&INSResponseLine::fkey_factura, factura->id)));
	
	if(! ins_response_line)
	{
		// auto whereClause = (c(&INSResponseLine::fkey_factura) == factura->id); // && (c(&INSResponseLine::fkey_INSResponse) == ins_response->id);
		if( count == 1)		// factura already associated with INSResponseLine
		{
			return;	// cannot add another one
		}
	}
#endif

	if (!ins_response_line)	// insert
	{
		ins_response_line = m_INSResponseLinesLB.insert(ins_response->id, factura->id, monto_cubierto, deducciones, copago, coaseguros, deducible_anual , total_a_pagar, monto_cubierto_p100, factura_cubierta_p100);
		m_INSResponseLinesLB.insert_into_listbox(*ins_response_line);
	}
	else                // update
	{
		ins_response_line->fkey_INSResponse = ins_response->id;
		ins_response_line->coaseguros = coaseguros;
		ins_response_line->copago = copago;
		ins_response_line->deducciones = deducciones;
		ins_response_line->deducible_anual = deducible_anual;
		ins_response_line->fkey_factura = factura->id;
		ins_response_line->monto_cubierto = monto_cubierto;
		ins_response_line->porcentaje_de_monto_cubierto = monto_cubierto_p100;
		ins_response_line->porcentaje_de_factura_cubierto = factura_cubierta_p100;
		ins_response_line->total_rubro_factura = total_a_pagar;
		m_INSResponseLinesLB.update(*ins_response_line);
	}
	m_INSResponseLinesLB.loadLBOrderByDesc(&INSResponseLine::id);
	m_INSResponseLine= ins_response_line;
	setIdFromRecord<INSResponseLine>(m_id_response_line, ins_response_line->id);
}


void INSResponseLineDlg::OnBnClickedNuevo()
{
	// TODO: Add your control notification handler code here
	m_INSResponseLine = std::nullopt;

	m_INSResponseLinesLB.select(std::nullopt);
	m_ins_responseCB.select(std::nullopt);
	m_ins_response = std::nullopt;
	m_invoicesCB.select(std::nullopt);
	m_invoice = std::nullopt;
	SetText(m_id_response_line, ""s);
	SetAmount(m_monto_cubierto, 0);
	SetAmount(m_deducciones, 0);
	SetAmount(m_copago, 0);
	SetAmount(m_coaseguros, 0);
	SetAmount(m_deducible_anual, 0);
	SetAmount(m_total_pagar, 0);
	SetAmount(m_monto_factura, 0);
	SetAmount(m_monto_cubierto_p100, 0);
	SetAmount(m_factura_cubierta_p100, 0);
	SetAmount(m_tipo_cambio, 0);
}


void INSResponseLineDlg::OnBnClickedBorrar()
{
	// TODO: Add your control notification handler code here
	if( m_INSResponseLinesLB.delete_current_sel())
	{
		OnBnClickedNuevo();
	}
}


void INSResponseLineDlg::OnBnClickedCerrar()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void INSResponseLineDlg::OnCbnSelchangeCFkeyInsresponse()
{
	// TODO: Add your control notification handler code here
	m_ins_response = m_ins_responseCB.current();
	SetAmount(m_tipo_cambio, m_ins_response->tipo_cambio);
	OnBnClickedCalculate();
}


void INSResponseLineDlg::OnCbnSelchangeCFkeyInvoice()
{
	// TODO: Add your control notification handler code here
	m_invoice = m_invoicesCB.current();
	SetAmount(m_monto_factura, m_invoice->amount);
	OnBnClickedCalculate();
}


void INSResponseLineDlg::OnEnChangeEMontoAPagar()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// // TODO:  Add your control notification handler code here
	// if(m_invoice && m_ins_response)
	// {
	// 	auto total_pagar = GetAmount(m_total_pagar);
	// 	auto monto_cubierto = GetAmount(m_monto_cubierto);
	// 	if (monto_cubierto == 0)   return;
	//
	// 	auto factura_cubierto_p100 = total_pagar* m_ins_response->tipo_cambio / m_invoice->amount * 100;
	// 	auto monto_cubierto_p100 = total_pagar / monto_cubierto * 100;
	// 	SetAmount(m_monto_cubierto_p100, monto_cubierto_p100);
	// 	SetAmount(m_factura_cubierta_p100, factura_cubierto_p100);
	// }
}


void INSResponseLineDlg::OnLbnSelchangeLInsresponseline()
{
	// TODO: Add your control notification handler code here
	m_INSResponseLine = m_INSResponseLinesLB.current();
	if (!m_INSResponseLine)  return;

	UpdateDlgControls();
#if 0
	m_ins_responseCB.select(m_INSResponseLine->fkey_INSResponse);
	m_ins_response = m_ins_responseCB.current();
	m_invoicesCB.select(m_INSResponseLine->fkey_factura);
	m_invoice = m_invoicesCB.current();
	SetAmount(m_id_response_line, m_INSResponseLine->id);
	SetAmount(m_monto_cubierto, m_INSResponseLine->monto_cubierto);
	SetAmount(m_deducciones, m_INSResponseLine->deducciones);
	SetAmount(m_copago, m_INSResponseLine->copago);
	SetAmount(m_coaseguros, m_INSResponseLine->coaseguros);
	SetAmount(m_deducible_anual, m_INSResponseLine->deducible_anual);
	SetAmount(m_total_pagar, m_INSResponseLine->total_rubro_factura);
	SetAmount(m_monto_factura, m_invoice->amount);
	SetAmount(m_monto_cubierto_p100, m_INSResponseLine->porcentaje_de_monto_cubierto);
	SetAmount(m_factura_cubierta_p100, m_INSResponseLine->porcentaje_de_factura_cubierto);
	SetAmount(m_tipo_cambio, m_ins_response->tipo_cambio);
#endif
}

void INSResponseLineDlg::UpdateDlgControls()
{
	m_ins_responseCB.select(m_INSResponseLine->fkey_INSResponse);
	m_ins_response = m_ins_responseCB.current();
	m_invoicesCB.select(m_INSResponseLine->fkey_factura);
	m_invoice = m_invoicesCB.current();
	SetAmount(m_id_response_line, m_INSResponseLine->id);
	SetAmount(m_monto_cubierto, m_INSResponseLine->monto_cubierto);
	SetAmount(m_deducciones, m_INSResponseLine->deducciones);
	SetAmount(m_copago, m_INSResponseLine->copago);
	SetAmount(m_coaseguros, m_INSResponseLine->coaseguros);
	SetAmount(m_deducible_anual, m_INSResponseLine->deducible_anual);
	SetAmount(m_total_pagar, m_INSResponseLine->total_rubro_factura);
	SetAmount(m_monto_factura, m_invoice->amount);
	SetAmount(m_monto_cubierto_p100, m_INSResponseLine->porcentaje_de_monto_cubierto);
	SetAmount(m_factura_cubierta_p100, m_INSResponseLine->porcentaje_de_factura_cubierto);
	SetAmount(m_tipo_cambio, m_ins_response->tipo_cambio);
}

void INSResponseLineDlg::OnBnClickedCalculate()
{
	// TODO: Add your control notification handler code here
	if (m_invoice && m_ins_response)
	{
		// auto total_pagar = GetAmount(m_total_pagar);
		auto monto_cubierto = GetAmount(m_monto_cubierto);
		if (monto_cubierto == 0)
		{
			MessageBoxW(L"Falta monto cubierto");
			return;
		}
		if(m_ins_response->tipo_cambio == 0)
		{
			MessageBoxW(L"Falta tipo de cambio");
			return;
		}
		auto monto_cubierto_colones = monto_cubierto * m_ins_response->tipo_cambio;
		auto deducciones = GetAmount(m_deducciones);
		auto copago = GetAmount(m_copago);
		auto coaseguro = GetAmount(m_coaseguros);
		auto deducible_anual = GetAmount(m_deducible_anual);

		auto total_pagar_dolares = monto_cubierto - deducciones - copago - coaseguro - deducible_anual;
		auto total_pagar = total_pagar_dolares * m_ins_response->tipo_cambio;
		SetAmount(m_total_pagar_colones, total_pagar);
		SetAmount(m_total_pagar, total_pagar_dolares);
		SetAmount(m_monto_factura, m_invoice->amount);

		auto factura_cubierto_p100 = total_pagar / m_invoice->amount * 100;
		auto monto_cubierto_p100 = total_pagar / monto_cubierto_colones * 100;
		SetAmount(m_monto_cubierto_p100, monto_cubierto_p100);
		SetAmount(m_factura_cubierta_p100, factura_cubierto_p100);
	}
}


void INSResponseLineDlg::OnBnClickedBInsResponses()
{
	// TODO: Add your control notification handler code here
	INSResponseDlg dlg;
	auto ins_resp = m_ins_responseCB.current();
	dlg.m_id = ins_resp ? ins_resp->id : -1;
	dlg.DoModal();
	Refresh();
}


void INSResponseLineDlg::OnBnClickedBInvoices()
{
	// TODO: Add your control notification handler code here
	InvoiceDlg dlg;
	dlg.DoModal();
	Refresh();
}


void INSResponseLineDlg::OnBnClickedBBringFromInsresponse()
{
	// TODO: Add your control notification handler code here
	if (!m_ins_response /* || !m_INSResponseLine*/ )	return;

	m_INSResponseLine->monto_cubierto = m_ins_response->total_bruto;
	m_INSResponseLine->coaseguros = m_ins_response->coaseguros;
	m_INSResponseLine->copago = m_ins_response->copagos;
	m_INSResponseLine->deducciones = m_ins_response->otras_deducciones;
	m_INSResponseLine->deducible_anual = m_ins_response->deducible_anual;
	m_INSResponseLine->fkey_INSResponse = m_ins_responseCB.current()->id; // m_ins_response->id;
	//m_INSResponseLine->id = -1;
	m_INSResponseLine->fkey_factura = m_invoicesCB.current()->id;
	// m_INSResponseLine->porcentaje_de_factura_cubierto =
	// m_INSResponseLine->porcentaje_de_monto_cubierto =
	// m_INSResponseLine->total_rubro_factura =
	UpdateDlgControls();
}
