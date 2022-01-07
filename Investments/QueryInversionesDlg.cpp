// QueryInversionesDlg.cpp : implementation file
//

#include "pch.h"
#include "Investments.h"
#include "afxdialogex.h"
#include "QueryInversionesDlg.h"

import Util;

// QueryInversionesDlg dialog

IMPLEMENT_DYNAMIC(QueryInversionesDlg, CDialog)

QueryInversionesDlg::QueryInversionesDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_QueryInversionesDlg, pParent),
	m_list_fondosCB(m_lista_fondos, [](Fondo& fondo)
	{
			return Util::to_cstring(fondo.simple_dump());
	})
{

}

QueryInversionesDlg::~QueryInversionesDlg()
{
}

void QueryInversionesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DT_FECHA_INVERSION, m_fecha_calculo);
	DDX_Control(pDX, IDC_CB_FONDOS, m_lista_fondos);
	DDX_Control(pDX, IDC_E_INVERSION_NUM_PARTICIPACIONES, m_num_participaciones);
}


BEGIN_MESSAGE_MAP(QueryInversionesDlg, CDialog)
	ON_BN_CLICKED(ID_B_CALCULATE, &QueryInversionesDlg::OnBnClickedBCalculate)
END_MESSAGE_MAP()


// QueryInversionesDlg message handlers


void QueryInversionesDlg::OnBnClickedBCalculate()
{
	// TODO: Add your control notification handler code here
	auto fecha = GetDate(m_fecha_calculo);
	auto fondo = m_list_fondosCB.current();
	if(!fondo)
	{
		MessageBoxW(L"Fondo no escogido!");
		return;
	}

	int num_part = Inversion::num_participaciones_en(fondo->id, fecha);

}
