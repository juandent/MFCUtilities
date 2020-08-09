// ConceptosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "ConceptosDlg.h"
#include "afxdialogex.h"
#include "CuentaDlg.h"
#include "RecordLinks.h"


// ConceptosDlg dialog

IMPLEMENT_DYNAMIC(ConceptosDlg, CDialog)

ConceptosDlg::ConceptosDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ConceptosDlg, pParent),
	m_conceptosLB{ m_listConceptos, [](Concepto& concepto)
	{
		return JD::to_cstring(concepto.id_concepto) + L" - " + JD::to_cstring(concepto.name) + L" - " + JD::to_cstring(concepto.fkey_account);
	}},
	m_accountsCB{ m_listAccounts, [](Account& account)
	{
		return JD::to_cstring(account.id_account) + L" - " + JD::to_cstring(account.number);
	}}
{

}

ConceptosDlg::~ConceptosDlg()
{
}

void ConceptosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_C_ACCOUNT, m_listAccounts);
	DDX_Control(pDX, IDC_L_CONCEPTOS, m_listConceptos);
	DDX_Control(pDX, IDC_E_NOMBRE_CONCEPTO, m_nombre);
	DDX_Control(pDX, IDC_E_ID_CONCEPTO, m_id);
}


BEGIN_MESSAGE_MAP(ConceptosDlg, CDialog)
	ON_BN_CLICKED(ID_B_BORRAR, &ConceptosDlg::OnBnClickedBBorrar)
	ON_BN_CLICKED(IDC_B_ADD_ACCOUNT, &ConceptosDlg::OnBnClickedBAddAccount)
	ON_BN_CLICKED(IDC_B_UPDATE_CONCEPTO, &ConceptosDlg::OnBnClickedBUpdateConcepto)
	ON_LBN_SELCHANGE(IDC_L_CONCEPTOS, &ConceptosDlg::OnLbnSelchangeLConceptos)
	ON_BN_CLICKED(IDC_B_APLICAR_CONCEPTO, &ConceptosDlg::OnBnClickedBAplicarConcepto)
END_MESSAGE_MAP()


// ConceptosDlg message handlers


BOOL ConceptosDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_conceptosLB.loadLB();
	m_accountsCB.loadLB();
	m_nombre.SetWindowTextW(JD::to_cstring(m_discriminator));
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void ConceptosDlg::OnBnClickedBBorrar()
{
	// TODO: Add your control notification handler code here
#if 0
	auto concepto = m_conceptosLB.current();

	if( ! concepto)
	{
		MessageBoxW(L"Falta escoger el concepto");
		return;
	}

	bool has_links = RecordLinks::has_links(*concepto);
	if (!has_links)
	{
		m_conceptosLB.delete_current_sel();
	}
#else
	m_conceptosLB.delete_current_sel();
#endif
}


void ConceptosDlg::OnBnClickedBAddAccount()
{
	// TODO: Add your control notification handler code here
	CuentaDlg dlg;
	dlg.DoModal();
	m_accountsCB.loadLB();
}


void ConceptosDlg::OnBnClickedBUpdateConcepto()
{
#if 0
	// TODO: Add your control notification handler code here
	std::optional<Concepto> concepto = m_conceptosLB.current();
	if (! concepto)
	{
		MessageBoxW(L"Falta escoger el concepto");
		return;
	}

	bool changes = false;
	CString rNombre;
	m_nombre.GetWindowTextW(rNombre);

	if (!rNombre.IsEmpty())
	{
		auto nombre = JD::from_cstring(rNombre);
		concepto->name = nombre;
		changes = true;
	}

	auto cuenta = m_accountsCB.current();
	if( cuenta)
	{
		concepto->fkey_account = cuenta->id_account;
		changes = true;
	}

	if (!changes)
	{
		MessageBox(L"No hay cambios en este objeto");
		return;
	}

	m_conceptosLB.update(*concepto);
	m_conceptosLB.loadLB();

	m_concepto = concepto;
#endif
}


void ConceptosDlg::OnLbnSelchangeLConceptos()
{
	// TODO: Add your control notification handler code here
	auto concepto = m_conceptosLB.current();

	m_nombre.SetWindowTextW(JD::to_cstring(concepto->name));
	m_accountsCB.select(concepto->fkey_account);
	m_id.SetWindowTextW(JD::to_cstring(concepto->id_concepto));
}


void ConceptosDlg::OnBnClickedBAplicarConcepto()
{
	// TODO: Add your control notification handler code here
	CString rNombre;
	m_nombre.GetWindowTextW(rNombre);

	auto nombre = JD::from_cstring(rNombre);
	if (rNombre.IsEmpty())
	{
		MessageBox(L"Falta escoger nombre");
		return;
	}

	auto cuenta = m_accountsCB.current();
	if (! cuenta)
	{
		MessageBox(L"Falta escoger cuenta");
		return;
	}

	auto whereClause = (c(&Concepto::name) == nombre.c_str());

	std::optional<Concepto> concepto_db = m_conceptosLB.exists(whereClause, &Concepto::id_concepto, &Concepto::name);

	if (!concepto_db)    // insert
	{
		concepto_db = m_conceptosLB.insert(nombre, cuenta->id_account);
		m_conceptosLB.insert_into_listbox(*concepto_db);
	}
	else               // update
	{
		concepto_db->name = nombre;
		concepto_db->fkey_account = cuenta->id_account;
		m_conceptosLB.update(*concepto_db);
	}
	m_conceptosLB.loadLB();
	m_concepto = concepto_db;
	m_id.SetWindowTextW(JD::to_cstring( m_concepto->id_concepto));
}
