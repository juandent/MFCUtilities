// CuentaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "CuentaDlg.h"
#include "afxdialogex.h"
#include "BancoDlg.h"
#include "Data_Tier.h"
#include "DuenosDlg.h"


// CuentaDlg dialog

IMPLEMENT_DYNAMIC(CuentaDlg, CDialog)

CuentaDlg::CuentaDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CuentaDlg, pParent),
	m_bancosCB{ m_banco_combo, [](Banco& banco)
	{
		return JD::to_cstring(banco.id_bank) + L" - " + JD::to_cstring(banco.nombre) + L" - " + JD::to_cstring(banco.ubicacion);
	} },
	m_ownerCB{ m_dueno_combo, [](AccountOwner& owner)
	{
			return JD::to_cstring(owner.name);
	} },
	m_cuentasLB{ m_list_cuentas, [](Account& account)
	{
		return JD::to_cstring(account.id_account) + L" - " + JD::to_cstring(account.number) + L" - " + (account.is_tarjeta ? L"Tarjeta" : L"Cuenta Bancaria");
	} }
{

}

CuentaDlg::~CuentaDlg()
{
}

void CuentaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID_CUENTA, m_id_cuenta);
	DDX_Control(pDX, IDC_E_DESCRIPTION, m_descripcion);
	DDX_Control(pDX, IDC_E_NUMERO, m_numero);
	DDX_Control(pDX, IDC_C_DUENO, m_dueno_combo);
	DDX_Control(pDX, IDC_R_TARJETA, m_tarjeta);
	DDX_Control(pDX, IDC_L_CUENTAS, m_list_cuentas);
	DDX_Control(pDX, IDC_C_BANCOS, m_banco_combo);
	DDX_Control(pDX, IDC_R_CUENTA_BANCARIA, m_cuenta_bancaria);
}


BEGIN_MESSAGE_MAP(CuentaDlg, CDialog)
	ON_BN_CLICKED(IDC_B_ADD_DUENOS, &CuentaDlg::OnBnClickedBAddDuenos)
	ON_BN_CLICKED(IDOK, &CuentaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_B_ADD_BANCOS, &CuentaDlg::OnBnClickedBAddBancos)
	ON_BN_CLICKED(IDC_B_APLICAR_CUENTA, &CuentaDlg::OnBnClickedBAplicarCuenta)
END_MESSAGE_MAP()


// CuentaDlg message handlers


BOOL CuentaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_bancosCB.loadLB();
	m_ownerCB.loadLB();
	m_cuentasLB.loadLB();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CuentaDlg::OnBnClickedBAddDuenos()
{
	// TODO: Add your control notification handler code here
	DuenosDlg dlg;
	dlg.DoModal();
	m_ownerCB.loadLB();
}


void CuentaDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CuentaDlg::OnBnClickedBAddBancos()
{
	// TODO: Add your control notification handler code here
	BancoDlg dlg;
	dlg.DoModal();
	m_bancosCB.loadLB();
}



void CuentaDlg::OnBnClickedBAplicarCuenta()
{
	// TODO: Add your control notification handler code here
	CString rNumero, rDescripcion;
	m_numero.GetWindowTextW(rNumero);
	m_descripcion.GetWindowTextW(rDescripcion);
	bool is_tarjeta_checked = m_tarjeta.GetCheck();
	bool is_cuenta_bancaria_checked = m_cuenta_bancaria.GetCheck();

	bool chosen_type = is_tarjeta_checked ^ is_cuenta_bancaria_checked;
	if (! chosen_type )
	{
		MessageBoxW(L"Falta escoger el tipo de cuenta");
		return;
	}

	if (rNumero.IsEmpty() || rDescripcion.IsEmpty())
	{
		MessageBoxW(L"Falta el numero de cuenta o su descripción");
		return;
	};

	auto numero = JD::from_cstring(rNumero);
	auto descripcion = JD::from_cstring(rDescripcion);

	auto whereClause = (c(&Account::number ) == numero.c_str());

	std::optional<Account> account = m_cuentasLB.exists(whereClause, &Account::id_account, &Account::number);

	if (!account)	// new account
	{
		// need to get current
		std::optional<AccountOwner> owner = m_ownerCB.current();
		if (! owner)
		{
			MessageBoxW(L"Falta escoger el dueño");
			return;
		}
		std::optional<Banco> banco = m_bancosCB.current();
		if( ! banco)
		{
			MessageBoxW(L"Falta escoger el banco");
			return;
		}

		account = m_cuentasLB.insert(numero, banco->id_bank, owner->id_owner, descripcion, is_tarjeta_checked);
		m_cuentasLB.insert_into_listbox(*account);
	}
}

