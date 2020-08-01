// BancoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "BancoDlg.h"
#include "afxdialogex.h"
#include "Data_Tier.h"
#include "PaisesDlg.h"


// BancoDlg dialog

IMPLEMENT_DYNAMIC(BancoDlg, CDialog)

BancoDlg::BancoDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BancoDlg, pParent),
m_bancosLB{m_list_bancos, []( Banco& banco)
{
	return JD::to_cstring(banco.id_bank) + L" - " + JD::to_cstring(banco.nombre) + L" - " + JD::to_cstring(banco.ubicacion) + L" - " + JD::to_cstring(banco.getPais().name);
}},
m_paisCB{ m_paises, [](Pais& pais)
{
	return JD::to_cstring(pais.name);
}}
{

}

BancoDlg::~BancoDlg()
{
}

void BancoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_NOMBRE_BANCO, m_nombre_banco);
	DDX_Control(pDX, IDC_L_BANCOS, m_list_bancos);
	DDX_Control(pDX, IDC_C_PAIS, m_paises);
	DDX_Control(pDX, IDC_E_UBICACION, m_ubicacion);
}


BEGIN_MESSAGE_MAP(BancoDlg, CDialog)
	ON_BN_CLICKED(IDC_B_ADD_PAIS, &BancoDlg::OnBnClickedBAddPais)
	ON_BN_CLICKED(IDC_B_APLICAR_BANCO, &BancoDlg::OnBnClickedBAplicarBanco)
	ON_BN_CLICKED(IDC_B_UPDATE_BANCO, &BancoDlg::OnBnClickedBUpdateBanco)
END_MESSAGE_MAP()


// BancoDlg message handlers


BOOL BancoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_paisCB.loadLB();
	m_bancosLB.loadLB();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void BancoDlg::OnBnClickedBAddPais()
{
	// TODO: Add your control notification handler code here
	PaisesDlg dlg;
	dlg.DoModal();
	m_paisCB.loadLB();
}


void BancoDlg::OnBnClickedBAplicarBanco()
{
	// TODO: Add your control notification handler code here
	CString rBanco, rUbicacion;
	m_nombre_banco.GetWindowTextW(rBanco);
	m_ubicacion.GetWindowTextW(rUbicacion);

	if (rBanco.IsEmpty() || rUbicacion.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre del banco o su ubicacion");
		return;
	};


	auto name = JD::from_cstring(rBanco);
	auto ubicacion = JD::from_cstring(rUbicacion);

	auto whereClause = (c(&Banco::nombre) == name.c_str()) && (c(&Banco::ubicacion) == ubicacion.c_str());

	std::optional<Banco> banco = m_bancosLB.exists(whereClause, &Banco::id_bank, &Banco::nombre);

	if (!banco)
	{
		// need to get current pais
		std::optional<Pais> pais = m_paisCB.current();
		if (pais)
		{
			banco = m_bancosLB.insert(name, ubicacion, pais->id_pais);
			m_bancosLB.insert_into_listbox(*banco);
		}
		else
		{
			MessageBoxW(L"Falta escoger el pais");
		}
	}
}


void BancoDlg::OnBnClickedBUpdateBanco()
{
	// TODO: Add your control notification handler code here
	std::optional<Banco> banco = m_bancosLB.current();
	if( ! banco)
	{
		MessageBoxW(L"Falta escoger el banco");
		return;
	}
	auto whereClause = (c(&Banco::id_bank) == banco->id_bank);

	bool changes = false;
	CString rNombreBanco;
	m_nombre_banco.GetWindowTextW(rNombreBanco);
	if( ! rNombreBanco.IsEmpty())
	{
		auto nombre = JD::from_cstring(rNombreBanco);
		banco->nombre = nombre;
		changes = true;
	}
	CString rUbicacion;
	m_ubicacion.GetWindowTextW(rUbicacion);
	if( ! rUbicacion.IsEmpty())
	{
		auto ubicacion = JD::from_cstring(rUbicacion);
		banco->ubicacion = ubicacion;
		changes = true;
	}

	auto pais = m_paisCB.current();
	if( pais )
	{
		banco->fkey_pais = pais->id_pais;
		changes = true;
	}
	
	if( ! changes)
	{
		MessageBox(L"No hay cambios en este objeto");
		return;
	}

	m_bancosLB.update(*banco);
	m_bancosLB.loadLB();
}
