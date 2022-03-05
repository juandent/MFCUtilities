// DuenoCuentasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "DuenoCuentasDlg.h"
#include "afxdialogex.h"
#include "CuentaDlg.h"


// DuenoCuentasDlg dialog

IMPLEMENT_DYNAMIC(DuenoCuentasDlg, CDialog)

DuenoCuentasDlg::DuenoCuentasDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DuenoCuentasDlg, pParent),
OneToN{ m_ownersName_box, [](AccountOwner& owner)
	{
		return Util::to_cstring(owner.id_owner) + L" " + Util::to_cstring(owner.name);
	}, m_cuentas_list, [](Account& account)
		{
				return Util::to_cstring(account.number);
		} }
{

}

DuenoCuentasDlg::~DuenoCuentasDlg()
{
}

void DuenoCuentasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OWNER_NAME, m_ownersName_box);
	DDX_Control(pDX, IDC_L_ACCOUNTS, m_cuentas_list);
}


BEGIN_MESSAGE_MAP(DuenoCuentasDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_OWNER_NAME, &DuenoCuentasDlg::OnCbnSelchangeOwnerName)
	ON_LBN_SELCHANGE(IDC_L_ACCOUNTS, &DuenoCuentasDlg::OnLbnSelchangeLAccounts)
END_MESSAGE_MAP()


// DuenoCuentasDlg message handlers


BOOL DuenoCuentasDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	OneToN.OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void DuenoCuentasDlg::OnCbnSelchangeOwnerName()
{
	// TODO: Add your control notification handler code here
	OneToN.On1SelectionChanged();
}


void DuenoCuentasDlg::OnLbnSelchangeLAccounts()
{
	// TODO: Add your control notification handler code here

	auto account = OneToN.OnNSelectionChanged();
	if (!account) return;

	CuentaDlg dlg;
	dlg.set_discriminator(account);
	dlg.DoModal();
}
