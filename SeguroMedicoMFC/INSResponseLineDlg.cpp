// INSResponseLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "INSResponseLineDlg.h"
#include "afxdialogex.h"


// INSResponseLineDlg dialog

IMPLEMENT_DYNAMIC(INSResponseLineDlg, CDialog)

INSResponseLineDlg::INSResponseLineDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_m_INSResponseLineDlg, pParent),
m_INSResponseLinesLB(m_list_INSResponseLines, [](INSResponseLine& line)
{
		return Util::to_cstring(line.simple_dump());
}),
m_ins_responseCB(m_list_ins_response, [](INSResponse& response)
{
		return Util::to_cstring(response.simple_dump());
}),
m_invoicesCB(m_list_facturas, [](Invoice& factura)
{
		return Util::to_cstring(factura.simple_dump());
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
}


BEGIN_MESSAGE_MAP(INSResponseLineDlg, CDialog)
END_MESSAGE_MAP()


// INSResponseLineDlg message handlers


BOOL INSResponseLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void INSResponseLineDlg::Refresh()
{
	m_INSResponseLinesLB.loadLBOrderBy(&INSResponseLine::monto_cubierto);
	m_ins_responseCB.loadLBOrderBy(&INSResponse::total_a_pagar);
	m_invoicesCB.loadLBOrderBy(&Invoice::amount);
}


