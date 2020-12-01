// PaisesDlg.cpp : implementation file
//

#include "stdafx.h"

import Util;


#include "ExpenseManagerSQLite.h"
#include "PaisesDlg.h"
#include "afxdialogex.h"
#include "RecordLinks.h"


// PaisesDlg dialog

IMPLEMENT_DYNAMIC(PaisesDlg, CDialog)

PaisesDlg::PaisesDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PaisesDlg, pParent),
	m_paisLB{ m_list_paises, [](Pais& pais)
	{
		return Util::to_cstring(pais.id_pais) + L" - " + Util::to_cstring(pais.name);
	}}
{

}

PaisesDlg::~PaisesDlg()
{
}

void PaisesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_L_PAISES, m_list_paises);
	DDX_Control(pDX, IDC_E_ID_PAIS, m_id_pais);
	DDX_Control(pDX, IDC_E_NOMBRE_PAIS, m_nombre_pais);
}


BEGIN_MESSAGE_MAP(PaisesDlg, CDialog)
	ON_BN_CLICKED(IDOK, &PaisesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_B_APLICAR_PAISES, &PaisesDlg::OnBnClickedBAplicarPaises)
	ON_LBN_SELCHANGE(IDC_L_PAISES, &PaisesDlg::OnLbnSelchangeLPaises)
	ON_BN_CLICKED(ID_B_BORRAR, &PaisesDlg::OnBnClickedBBorrar)
	ON_BN_CLICKED(IDC_B_UPDATE_PAIS, &PaisesDlg::OnBnClickedBUpdatePais)
END_MESSAGE_MAP()


// PaisesDlg message handlers


BOOL PaisesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_paisLB.loadLB();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void PaisesDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void PaisesDlg::OnBnClickedBAplicarPaises()
{
	// TODO: Add your control notification handler code here
	auto pais = getCurrent<Pais>(m_id_pais);

	CString rStr;
	m_nombre_pais.GetWindowTextW(rStr);

	if (rStr.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre del pais");
		return;
	}

	auto name = Util::from_cstring(rStr);

	if (!pais)
	{

		auto whereClause = c(&Pais::name) == name.c_str();
		std::optional<Pais> pais_by_value = m_paisLB.exists(whereClause, &Pais::id_pais, &Pais::name);
		if (pais_by_value)
		{
			MessageBoxW(L"Pais found for another primary key");
		}
		pais = pais_by_value;

	}
	if (!pais)    // insert
	{
		pais = m_paisLB.insert(name);
		m_paisLB.insert_into_listbox(*pais);
		m_id_pais.SetWindowTextW(Util::to_cstring(pais->id_pais));
	}
	else           // update
	{
		pais->name = name;
		m_paisLB.update(*pais);
	}
	m_paisLB.loadLB();
	m_pais = pais;

	setIdFromRecord<Pais>(m_id_pais, pais->id_pais);
}


void PaisesDlg::OnLbnSelchangeLPaises()
{
	// TODO: Add your control notification handler code here
	auto pais = m_paisLB.current();

	CString rNombre;

	m_nombre_pais.SetWindowTextW(Util::to_cstring(pais->name));
	m_id_pais.SetWindowTextW(Util::to_cstring(pais->id_pais));
}



void PaisesDlg::OnBnClickedBBorrar()
{
	// TODO: Add your control notification handler code here
#if 0
	auto pais = m_paisLB.current();
	if (pais)
	{
		bool has_links = RecordLinks::has_links(*pais);
		if (!has_links)
		{
			m_paisLB.delete_current_sel();
		}
	}
#else
	m_paisLB.delete_current_sel();
#endif
}



void PaisesDlg::OnBnClickedBUpdatePais()
{
#if 0
	// TODO: Add your control notification handler code here
	auto pais = m_paisLB.current();
	if( ! pais)
	{
		MessageBoxW(L"Falta escoger pais");
		return;
	}
	CString rPais;
	m_nombre_pais.GetWindowTextW(rPais);
	pais->name = Util::from_cstring(rPais);

	m_paisLB.update(*pais);
	m_paisLB.loadLB();
#endif
}
