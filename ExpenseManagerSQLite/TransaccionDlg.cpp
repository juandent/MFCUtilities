// TransaccionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "TransaccionDlg.h"
#include "afxdialogex.h"
#include "ConceptosDlg.h"


// TransaccionDlg dialog

IMPLEMENT_DYNAMIC(TransaccionDlg, CDialog)

TransaccionDlg::TransaccionDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TransaccionDlg, pParent),
	m_transaccionLB{ m_listTransactions, [](Transaccion& trans)
	{
		Colones colones = trans.amount_colones;
		Dolares dolares = trans.amount_dolares;
		return JD::to_cstring(trans.id_transaccion) + L" - " + JD::to_cstring(trans.line_date) + L" - " + JD::to_cstring(trans.descripcion) + L" - " + JD::to_cstring(colones) + L" - " + JD::to_cstring(dolares);
	}},
	m_conceptoCB{ m_list_concepto, [](Concepto& concepto)
	{
		return JD::to_cstring(concepto.name);
	}},
	m_statementCB{ m_list_statement, [](Statement& statement)
	{
		return JD::to_cstring(statement.date);
	}},
	m_categoriaCB{ m_list_category, [](Categoria& categoria)
	{
		return JD::to_cstring(categoria.name) + L" - " + (categoria.is_expense_or_income ? L"Expense or income" : L"*");
	}},
	m_own_accountCB{ m_list_own_accounts, [](Account& account)
	{
		return JD::to_cstring(account.number);
	}},
	m_other_accountCB{ m_list_other_account, [](Account& account)
	{
		return JD::to_cstring(account.number);
	}}
{

}

TransaccionDlg::~TransaccionDlg()
{
}

void TransaccionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_TRANSACTION, m_date_transaccion);
	DDX_Control(pDX, IDC_L_TRANSACTION, m_listTransactions);
	DDX_Control(pDX, IDC_C_CONCEPTO, m_list_concepto);
	DDX_Control(pDX, IDC_C_ESTADO_CUENTA, m_list_statement);
	DDX_Control(pDX, IDC_C_OWN_ACCOUNT, m_list_own_accounts);
	DDX_Control(pDX, IDC_C_OTHER_ACCOUNT, m_list_other_account);
	DDX_Control(pDX, IDC_C_CATEGORIAS, m_list_category);
	DDX_Control(pDX, IDC_E_MONTO_COLONES, m_colones);
	DDX_Control(pDX, IDC_E_MONTO_DOLARES, m_dolares);
	DDX_Control(pDX, IDC_E_DESCRIPCION, m_descripcion);
	DDX_Control(pDX, IDC_E_TRANSACCION, m_id);
}


BEGIN_MESSAGE_MAP(TransaccionDlg, CDialog)
	ON_BN_CLICKED(IDC_B_ADD_STATEMENT, &TransaccionDlg::OnBnClickedBAddStatement)
	ON_BN_CLICKED(IDC_B_ADD_CONCEPT, &TransaccionDlg::OnBnClickedBAddConcept)
	ON_BN_CLICKED(IDOK, &TransaccionDlg::OnBnClickedOk)
	ON_NOTIFY(MCN_SELCHANGE, IDC_DATE_TRANSACTION, &TransaccionDlg::OnMcnSelchangeDateTransaction)
	ON_CBN_SELCHANGE(IDC_C_ESTADO_CUENTA, &TransaccionDlg::OnCbnSelchangeCEstadoCuenta)
	ON_CBN_SELCHANGE(IDC_C_OTHER_ACCOUNT, &TransaccionDlg::OnCbnSelchangeCOtherAccount)
	ON_BN_CLICKED(IDC_B_APLICAR_TRANSACTIONS, &TransaccionDlg::OnBnClickedBAplicarTransactions)
END_MESSAGE_MAP()


// TransaccionDlg message handlers


BOOL TransaccionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_other_accountCB.loadLB();
	m_conceptoCB.loadLB();
	m_statementCB.loadLB();
	m_own_accountCB.loadLB();
	m_categoriaCB.loadLB();
	m_other_accountCB.loadLB();
	m_transaccionLB.loadLB();

	//m_date_transaccion.SetCurSel();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void TransaccionDlg::OnBnClickedBAddStatement()
{
	// TODO: Add your control notification handler code here
	
}


void TransaccionDlg::OnBnClickedBAddConcept()
{
	// TODO: Add your control notification handler code here
	ConceptosDlg dlg;
	dlg.DoModal();
}


void TransaccionDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void TransaccionDlg::OnMcnSelchangeDateTransaction(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void TransaccionDlg::OnCbnSelchangeCEstadoCuenta()
{
	// TODO: Add your control notification handler code here
	auto statement = m_statementCB.current();
	if( !statement )
	{
		MessageBoxW(L"Falta escojer el estado de cuenta");
		return;
	}
	auto whereClause = c(&Transaccion::fkey_statement) == statement->id_statement;
	m_transaccionLB.loadLB(whereClause);
}


void TransaccionDlg::OnCbnSelchangeCOtherAccount()
{
	// TODO: Add your control notification handler code here
}


void TransaccionDlg::OnBnClickedBAplicarTransactions()
{
	// TODO: Add your control notification handler code here
	using namespace date;
	
	CString rDescripcion;
	m_descripcion.GetWindowTextW(rDescripcion);
	auto descripcion = JD::from_cstring(rDescripcion);

	auto statement = m_statementCB.current();
	if( !statement)
	{
		MessageBox(L"Falta escoger estado de cuenta");
		return;
	}
	auto other_account = m_other_accountCB.current();
	if ( !other_account )
	{
		MessageBoxW(L"Falta escoger la otra cuenta");
		return;
	}

	auto own_account = m_own_accountCB.current();
	if( !own_account)
	{
		MessageBoxW(L"Falta escoger la cuenta propia");
		return;
	}

	auto concepto = m_conceptoCB.current();
	if( ! concepto)
	{
		MessageBoxW(L"Falta escoger el concepto");
		return;
	}
	auto categoria = m_categoriaCB.current();
	if( ! categoria)
	{
		MessageBoxW(L"Falta escoger la categoria");
		return;
	}
	COleDateTime rDateTime;
	bool ok_date = m_date_transaccion.GetCurSel(rDateTime);

	if( ! ok_date)
	{
		MessageBoxW(L"Falta escoger la fecha de la transaccion");
		return;
	}

	auto transaction_date = rDateTime.Format(L"%Y-%m-%d");
	int year_value = rDateTime.GetYear();
	unsigned int month_value = rDateTime.GetMonth();
	unsigned int day_value = rDateTime.GetDay();
	
	year_month_day ymd{ year{year_value}, month{month_value}, day{day_value} };
	sys_days line_date = ymd;

	CString rColones;
	m_colones.GetWindowTextW(rColones);
	CString rDolares;
	m_dolares.GetWindowTextW(rDolares);

	if( rColones.IsEmpty() && rDolares.IsEmpty())
	{
		MessageBox(L"Falta especificar el monto en colones o en dolares");
		return;
	}
	
	auto str_colones = JD::from_cstring(rColones);
	auto str_dolares = JD::from_cstring(rDolares);
	auto colones = std::stod(str_colones);
	auto dolares = std::stod(str_dolares);
	
	
	auto whereClause = (c(&Transaccion::fkey_concepto) == concepto->id_concepto) && (c(&Transaccion::line_date) == line_date) && (c(&Transaccion::fkey_statement) == statement->id_statement)
					&& (c(&Transaccion::amount_dolares) == dolares) && (c(&Transaccion::amount_colones) == colones) && (c(&Transaccion::fkey_account_other) == other_account->id_account)
					&& (c(&Transaccion::descripcion) == descripcion.c_str()) && (c(&Transaccion::fkey_account_own) == own_account->id_account) && (c(&Transaccion::fkey_category) == categoria->id_categoria);

	std::optional<Transaccion> trans = m_transaccionLB.exists(whereClause, &Transaccion::id_transaccion, &Transaccion::amount_colones, &Transaccion::amount_dolares, &Transaccion::fkey_account_own, &Transaccion::fkey_account_other, &Transaccion::line_date,
		&Transaccion::descripcion, &Transaccion::fkey_category, &Transaccion::fkey_concepto, &Transaccion::fkey_statement);

		// std::optional<Transaccion> trans = m_transaccionLB.exists(whereClause, &Transaccion::id_transaccion);

	if( !trans)
	{
		trans = m_transaccionLB.insert(colones, dolares, own_account->id_account, other_account->id_account, line_date, descripcion, categoria->id_categoria, concepto->id_concepto, statement->id_statement);
		m_transaccionLB.insert_into_listbox(*trans);
		m_id.SetWindowTextW(JD::to_cstring(trans->id_transaccion));
	}
}

