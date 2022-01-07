// InversionDlg.cpp : implementation file
//

#include "pch.h"
#include "Investments.h"
#include "afxdialogex.h"
#include "InversionDlg.h"

#include "CFondoDlg.h"

import Util;

// InversionDlg dialog

IMPLEMENT_DYNAMIC(InversionDlg, CDialog)

InversionDlg::InversionDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_InversionDlg, pParent),
	m_list_inversionesLB(m_lista_inversiones, [](Inversion& inv)
	{
			return Util::to_cstring(inv.simple_dump());
	}),
	m_list_fondosCB(m_lista_fondos, [](Fondo& fondo)
	{
			return Util::to_cstring(fondo.simple_dump());
	})
{

}

InversionDlg::~InversionDlg()
{
}

void InversionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_INVERSION_ID, m_inversion_id);
	DDX_Control(pDX, IDC_E_INVERSION_NUM_PARTICIPACIONES, m_num_participaciones);
	DDX_Control(pDX, IDC_DT_FECHA_INVERSION, m_fecha_inversion);
	DDX_Control(pDX, IDC_CB_FONDOS, m_lista_fondos);
	DDX_Control(pDX, IDC_LIST_INVERSIONES, m_lista_inversiones);
}


BEGIN_MESSAGE_MAP(InversionDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &InversionDlg::OnBnClickedApply)
	ON_LBN_SELCHANGE(IDC_LIST_INVERSIONES, &InversionDlg::OnLbnSelchangeListInversiones)
	ON_BN_CLICKED(ID_NEW, &InversionDlg::OnBnClickedNew)
	ON_BN_CLICKED(ID_ERASE, &InversionDlg::OnBnClickedErase)
	ON_BN_CLICKED(IDC_B_FONDOS, &InversionDlg::OnBnClickedBFondos)
END_MESSAGE_MAP()


// InversionDlg message handlers


BOOL InversionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void InversionDlg::Refresh()
{
	m_list_inversionesLB.loadLBOrderByDesc(&Inversion::beginning_date);
	m_list_fondosCB.loadLBOrderBy(&Fondo::nombre);
}


void InversionDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	m_inversion = getCurrent<Inversion>(m_inversion_id);

	auto fondo = m_list_fondosCB.current();
	if(!fondo)
	{
		MessageBoxW(L"Falta escoger Fondo");
		return;
	}

	auto num_part = GetInteger(m_num_participaciones);
	std::chrono::sys_days fecha = GetDate(m_fecha_inversion);

	if (!m_inversion)	// insert
	{
		m_inversion = m_list_inversionesLB.insert(num_part, fecha, fondo->id);
		m_list_inversionesLB.insert_into_listbox(*m_inversion);
	}
	else				// update
	{
		m_inversion->num_participaciones = num_part;
		m_inversion->beginning_date = fecha;
		m_inversion->fkey_fondo = fondo->id;
		m_list_inversionesLB.update(*m_inversion);
	}

	setIdFromRecord<Inversion>(m_inversion_id, m_inversion->id);
	Refresh();
}


void InversionDlg::OnLbnSelchangeListInversiones()
{
	// TODO: Add your control notification handler code here
	m_inversion = m_list_inversionesLB.current();
	if (!m_inversion)	return;
	SetText(m_inversion_id, m_inversion->id);
	m_list_fondosCB.select(m_inversion->fkey_fondo);
	SetDate(m_fecha_inversion, m_inversion->beginning_date);
	SetAmount(m_num_participaciones, m_inversion->num_participaciones);
}


void InversionDlg::OnBnClickedNew()
{
	// TODO: Add your control notification handler code here
	SetText(m_inversion_id, ""s);
	m_list_fondosCB.select(-1);

	using namespace std::chrono;
	const auto today = sys_days{ floor<days>(system_clock::now()) };

	SetDate(m_fecha_inversion, today );
	SetAmount(m_num_participaciones, 0);
	m_inversion = std::nullopt;
}


void InversionDlg::OnBnClickedErase()
{
	// TODO: Add your control notification handler code here
	if( m_list_inversionesLB.delete_current_sel())
	{
		OnBnClickedNew();
	}
}


void InversionDlg::OnBnClickedBFondos()
{
	// TODO: Add your control notification handler code here
	CFondoDlg dlg;
	dlg.DoModal();
	Refresh();
}
