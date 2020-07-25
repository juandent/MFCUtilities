// ConceptosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "ConceptosDlg.h"
#include "afxdialogex.h"
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
	DDX_Control(pDX, IDC_L_BANCOS, m_listConceptos);
}


BEGIN_MESSAGE_MAP(ConceptosDlg, CDialog)
	ON_BN_CLICKED(ID_B_BORRAR, &ConceptosDlg::OnBnClickedBBorrar)
END_MESSAGE_MAP()


// ConceptosDlg message handlers


BOOL ConceptosDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_conceptosLB.loadLB();
	m_accountsCB.loadLB();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void ConceptosDlg::OnBnClickedBBorrar()
{
	// TODO: Add your control notification handler code here
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

}
