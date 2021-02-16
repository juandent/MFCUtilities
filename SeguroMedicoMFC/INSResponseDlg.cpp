// INSResponseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "INSResponseDlg.h"
#include "afxdialogex.h"


// INSResponseDlg dialog

IMPLEMENT_DYNAMIC(INSResponseDlg, CDialog)

INSResponseDlg::INSResponseDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_INSResponseDlg, pParent)
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
}


BEGIN_MESSAGE_MAP(INSResponseDlg, CDialog)
END_MESSAGE_MAP()


// INSResponseDlg message handlers


BOOL INSResponseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
