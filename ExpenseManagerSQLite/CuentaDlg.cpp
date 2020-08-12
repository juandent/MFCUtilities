// CuentaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "CuentaDlg.h"
#include "afxdialogex.h"
#include "BancoDlg.h"
#include "Data_Tier.h"
#include "DuenosDlg.h"
#include "RecordLinks.h"

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
	ON_BN_CLICKED(ID_B_BORRAR_CUENTA, &CuentaDlg::OnBnClickedBBorrarCuenta)
	ON_BN_CLICKED(IDC_B_UPDATE_ACCOUNT, &CuentaDlg::OnBnClickedBUpdateAccount)
	ON_LBN_SELCHANGE(IDC_L_CUENTAS, &CuentaDlg::OnLbnSelchangeLCuentas)
	ON_EN_SETFOCUS(IDC_E_ID_CUENTA, &CuentaDlg::OnEnSetfocusEIdCuenta)
END_MESSAGE_MAP()


// CuentaDlg message handlers


BOOL CuentaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_numero.SetWindowTextW(JD::to_cstring(m_numero_val));
	m_bancosCB.loadLB();
	m_ownerCB.loadLB();
	m_cuentasLB.loadLB();

	if(m_account)
	{
		// loads all controls by responding to PostMessage
		m_cuentasLB.select(m_account->id_account);
	}
	
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

#if 0
std::optional<Account> CuentaDlg::getCurrent() const
{
	/// <summary>
	/// does cuenta exist already?
	/// </summary>
	CString rId;
	m_id_cuenta.GetWindowTextW(rId);
	auto id_str = JD::from_cstring(rId);
	auto id = std::stoi(id_str);
	auto cuenta = Storage::getStorage().get_optional<Account>(id);
	///////////	
	return cuenta;
}

Operation CuentaDlg::whatOperation() const
{
	auto cuenta = getCurrent();
	return cuenta ? Operation::doUpdate : Operation::doInsert;
}
#endif

void CuentaDlg::OnBnClickedBAplicarCuenta()
{
	// TODO: Add your control notification handler code here
	auto cuenta = getCurrent<Account>(m_id_cuenta);
	
		
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
	std::optional<AccountOwner> owner = m_ownerCB.current();
	if (!owner)
	{
		MessageBoxW(L"Falta escoger el dueño");
		return;
	}
	std::optional<Banco> banco = m_bancosCB.current();
	if (!banco)
	{
		MessageBoxW(L"Falta escoger el banco");
		return;
	}

	auto numero = JD::from_cstring(rNumero);
	auto descripcion = JD::from_cstring(rDescripcion);

	if (! cuenta) 
	{
		auto whereClause = (c(&Account::number) == numero.c_str());

		std::optional<Account> account_by_value = m_cuentasLB.exists(whereClause, &Account::id_account, &Account::number);
		if (account_by_value)
		{
			MessageBoxW(L"Cuenta found for another primary key");
		}
		cuenta = account_by_value;
	}
	if (!cuenta)	// new account
	{
		// need to get current
		cuenta = m_cuentasLB.insert(numero, banco->id_bank, owner->id_owner, descripcion, is_tarjeta_checked);
		m_cuentasLB.insert_into_listbox(*cuenta);
	}
	else      // update
	{
		cuenta->number = numero;
		cuenta->description = descripcion;
		cuenta->is_tarjeta = is_tarjeta_checked;
		cuenta->fkey_bank = banco->id_bank;
		cuenta->fkey_account_owner = owner->id_owner;
		m_cuentasLB.update(*cuenta);
	}
	m_cuentasLB.loadLB();
	m_account = cuenta;
	setIdFromRecord<Account>(m_id_cuenta, cuenta->id_account);
}



void CuentaDlg::OnBnClickedBBorrarCuenta()
{
	// TODO: Add your control notification handler code here
#if 0
	auto cuenta = m_cuentasLB.current();

	if (!cuenta)
	{
		MessageBoxW(L"Falta escoger la cuenta");
		return;
	}

	bool has_links = RecordLinks::has_links(*cuenta);
	if (!has_links)
	{
		m_cuentasLB.delete_current_sel();
	}
#else
	m_cuentasLB.delete_current_sel();
#endif
}


void CuentaDlg::OnBnClickedBUpdateAccount()
{
#if 0	
	// TODO: Add your control notification handler code here
	auto cuenta = m_cuentasLB.current();

	if (!cuenta)
	{
		MessageBoxW(L"Falta escoger la cuenta");
		return;
	}
	CString rNumero;
	m_numero.GetWindowTextW(rNumero);
	CString rDescripcion;
	m_descripcion.GetWindowTextW(rDescripcion);
	auto owner = m_ownerCB.current();
	auto banco = m_bancosCB.current();
	auto is_tarjeta = m_tarjeta.GetCheck();

	cuenta->number = JD::from_cstring(rNumero);
	cuenta->description = JD::from_cstring(rDescripcion);
	cuenta->is_tarjeta = is_tarjeta;
	cuenta->fkey_account_owner = owner->id_owner;
	cuenta->fkey_bank = banco->id_bank;

	m_cuentasLB.update(*cuenta);
	m_cuentasLB.loadLB();
#endif
}


void CuentaDlg::OnLbnSelchangeLCuentas()
{
	// TODO: Add your control notification handler code here
	auto cuenta = m_cuentasLB.current();

	m_numero.SetWindowTextW(JD::to_cstring(cuenta->number));
	m_descripcion.SetWindowTextW(JD::to_cstring(cuenta->description));
	m_id_cuenta.SetWindowTextW(JD::to_cstring(cuenta->id_account));

	m_ownerCB.select(cuenta->fkey_account_owner);
	m_bancosCB.select(cuenta->fkey_bank);
	m_tarjeta.SetCheck(cuenta->is_tarjeta);
	m_cuenta_bancaria.SetCheck(!cuenta->is_tarjeta);
}


void CuentaDlg::OnEnSetfocusEIdCuenta()
{
	// TODO: Add your control notification handler code here
	m_id_cuenta.SendMessage(WM_KILLFOCUS);
}
