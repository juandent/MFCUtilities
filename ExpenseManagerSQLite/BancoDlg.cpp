// BancoDlg.cpp : implementation file
//

#include "stdafx.h"

import Util;

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
	return Util::to_cstring(banco.id_bank) + L" - " + Util::to_cstring(banco.nombre) + L" - " + Util::to_cstring(banco.ubicacion) + L" - " + Util::to_cstring(banco.getPais().name);
}},
m_paisCB{ m_paises, [](Pais& pais)
{
	return Util::to_cstring(pais.name);
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
	DDX_Control(pDX, IDC_E_ID_BANCO, m_id_banco);
}


BEGIN_MESSAGE_MAP(BancoDlg, CDialog)
	ON_BN_CLICKED(IDC_B_ADD_PAIS, &BancoDlg::OnBnClickedBAddPais)
	ON_BN_CLICKED(IDC_B_APLICAR_BANCO, &BancoDlg::OnBnClickedBAplicarBanco)
	ON_BN_CLICKED(IDC_B_UPDATE_BANCO, &BancoDlg::OnBnClickedBUpdateBanco)
	ON_BN_CLICKED(ID_B_BORRAR, &BancoDlg::OnBnClickedBBorrar)
	ON_LBN_SELCHANGE(IDC_L_BANCOS, &BancoDlg::OnLbnSelchangeLBancos)
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
	auto banco = getCurrent<Banco>(m_id_banco);
	
	CString rBanco, rUbicacion;
	m_nombre_banco.GetWindowTextW(rBanco);
	m_ubicacion.GetWindowTextW(rUbicacion);

	if (rBanco.IsEmpty() || rUbicacion.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre del banco o su ubicacion");
		return;
	};

	// need to get current pais
	std::optional<Pais> pais = m_paisCB.current();
	if( ! pais)
	{
		MessageBoxW(L"Falta escoger el pais");
		return;
	}

	auto name = Util::from_cstring(rBanco);
	auto ubicacion = Util::from_cstring(rUbicacion);

	std::optional<Banco> banco_by_value;
	if (! banco)
	{
		auto whereClause = (c(&Banco::nombre) == name.c_str()) && (c(&Banco::ubicacion) == ubicacion.c_str());

		banco_by_value = m_bancosLB.exists(whereClause); // , & Banco::id_bank, & Banco::nombre);
		if( banco_by_value )
		{
			MessageBoxW(L"Banco found for another primary key");
		}
		banco = banco_by_value;
	}
	if (! banco )    // insert
 	{
		// need to get current pais
		banco = m_bancosLB.insert(name, ubicacion, pais->id_pais);
		m_bancosLB.insert_into_listbox(*banco);
	}
	else               // update
	{
		banco->nombre = name;
		banco->ubicacion = ubicacion;
		banco->fkey_pais = pais->id_pais;
		m_bancosLB.update(*banco);
	}
	m_bancosLB.loadLB();
	m_banco = banco;
	setIdFromRecord<Banco>(m_id_banco, banco->id_bank);
}


void BancoDlg::OnBnClickedBUpdateBanco()
{
#if 0
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
		auto nombre = Util::from_cstring(rNombreBanco);
		banco->nombre = nombre;
		changes = true;
	}
	CString rUbicacion;
	m_ubicacion.GetWindowTextW(rUbicacion);
	if( ! rUbicacion.IsEmpty())
	{
		auto ubicacion = Util::from_cstring(rUbicacion);
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
#endif
}


void BancoDlg::OnBnClickedBBorrar()
{
	// TODO: Add your control notification handler code here
	m_bancosLB.delete_current_sel();
}


void BancoDlg::OnLbnSelchangeLBancos()
{
	// TODO: Add your control notification handler code here
	auto banco = m_bancosLB.current();
	m_id_banco.SetWindowTextW(Util::to_cstring(banco->id_bank));
	m_nombre_banco.SetWindowTextW(Util::to_cstring(banco->nombre));
	m_ubicacion.SetWindowTextW(Util::to_cstring(banco->ubicacion));
	m_paisCB.select(banco->fkey_pais);
}
