// TransaccionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "TransaccionDlg.h"
#include "afxdialogex.h"
#include "ConceptosDlg.h"
#include "StatementDlg.h"
#include "RecordLinks.h"


// TransaccionDlg dialog

IMPLEMENT_DYNAMIC(TransaccionDlg, CDialog)

TransaccionDlg::TransaccionDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TransaccionDlg, pParent),
	m_transaccionLB{ m_listTransactions, [](Transaccion& trans)
	{
		Colones colones = trans.amount_colones;
		Dolares dolares = trans.amount_dolares;
		return JD::to_cstring(trans.id_transaccion) + L" - " + JD::to_cstring(trans.row) + L"# - " +			
			JD::to_cstring(trans.line_date) + L" - " + JD::to_cstring(trans.descripcion) + L" - " + JD::to_cstring(colones) + L" - " + JD::to_cstring(dolares);
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
	DDX_Control(pDX, IDC_E_ROW, m_row);
}


BEGIN_MESSAGE_MAP(TransaccionDlg, CDialog)
	ON_BN_CLICKED(IDC_B_ADD_STATEMENT, &TransaccionDlg::OnBnClickedBAddStatement)
	ON_BN_CLICKED(IDC_B_ADD_CONCEPT, &TransaccionDlg::OnBnClickedBAddConcept)
	ON_BN_CLICKED(IDOK, &TransaccionDlg::OnBnClickedOk)
	ON_NOTIFY(MCN_SELCHANGE, IDC_DATE_TRANSACTION, &TransaccionDlg::OnMcnSelchangeDateTransaction)
	ON_CBN_SELCHANGE(IDC_C_ESTADO_CUENTA, &TransaccionDlg::OnCbnSelchangeCEstadoCuenta)
	ON_CBN_SELCHANGE(IDC_C_OTHER_ACCOUNT, &TransaccionDlg::OnCbnSelchangeCOtherAccount)
	ON_BN_CLICKED(IDC_B_APLICAR_TRANSACTIONS, &TransaccionDlg::OnBnClickedBAplicarTransactions)
	ON_LBN_SELCHANGE(IDC_L_TRANSACTION, &TransaccionDlg::OnLbnSelchangeLTransaction)
	ON_BN_CLICKED(ID_B_BORRAR, &TransaccionDlg::OnBnClickedBBorrar)
	ON_BN_CLICKED(IDC_B_UPDATE_TRANSACTION, &TransaccionDlg::OnBnClickedBUpdateTransaction)
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

	if (m_trans)
	{
		const auto date = JD::to_ole_date_time(m_trans->line_date);
		m_date_transaccion.SetCurSel(date);
		
		m_conceptoCB.select(m_trans->fkey_concepto);
		m_categoriaCB.select(m_trans->fkey_category);

		Colones colones{ m_trans->amount_colones };
		CString str_colones = JD::to_cstring(colones);
		m_colones.SetWindowTextW(str_colones);

		Dolares dolares{ m_trans->amount_dolares };
		CString str_dolares = JD::to_cstring(dolares);
		m_dolares.SetWindowTextW(str_dolares);
		
		m_descripcion.SetWindowTextW(JD::to_cstring(m_trans->descripcion));
		m_statementCB.select(m_trans->fkey_statement);
		m_own_accountCB.select(m_trans->fkey_account_own);

		m_id.SetWindowTextW(JD::to_cstring(m_trans->id_transaccion));
#define MODIFY_SCHEMA
#ifdef MODIFY_SCHEMA
		m_row.SetWindowTextW(JD::to_cstring(m_trans->row));
#endif
	}
	if( m_autoexec)
	{
		this->OnBnClickedBAplicarTransactions();
		this->OnBnClickedOk();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void TransaccionDlg::OnBnClickedBAddStatement()
{
	// TODO: Add your control notification handler code here
	StatementDlg dlg;
	dlg.DoModal();
	m_statementCB.loadLB();
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

#if 0
namespace JD
{
	Money strip_to_money(std::string moneyAsString)
	{
		std::string stripped;
		for( auto& c : moneyAsString)
		{
			if( c != '$' &&  c != '¢' && c != ',')
			{
				stripped += c;
			}
		}
		return JD::to_money(stripped);
		
	}
}
#endif

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
		// MessageBoxW(L"Falta escoger la otra cuenta");
		// return;
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
	sys_days line_date = JD::to_sys_days(rDateTime);

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
	auto dolares = JD::strip_to_long_double(str_dolares);
	auto colones = JD::strip_to_long_double(str_colones);

	std::optional<int> account_other;
	if (other_account)
	{
		account_other = other_account->id_account;
	}
	else
	{
		account_other = std::nullopt;
	}



#if 0
	CString rID;
	m_id.GetWindowTextW(rID);
	auto id_str = JD::from_cstring(rID);
	auto id = stoi(id_str);
	
	std::optional<Transaccion> trans = Storage::getStorage().get_optional<Transaccion>(id);
#else
	CString rRow;
	m_row.GetWindowTextW(rRow);
	auto row_str = JD::from_cstring(rRow);
	auto row = stoi(row_str);
	auto whereClause = (c(&Transaccion::row) == row) && (c(&Transaccion::fkey_statement) == statement->id_statement);
	std::optional<Transaccion> trans = m_transaccionLB.exists(whereClause, &Transaccion::id_transaccion, &Transaccion::row, &Transaccion::fkey_statement);

#endif
#if 0
	if (other_account)
	{
		auto whereClause = (c(&Transaccion::fkey_concepto) == concepto->id_concepto) && (c(&Transaccion::line_date) == line_date) && (c(&Transaccion::fkey_statement) == statement->id_statement)
			&& (c(&Transaccion::amount_dolares) == dolares) && (c(&Transaccion::amount_colones) == colones) && (c(&Transaccion::fkey_account_other) == account_other)
			&& (c(&Transaccion::descripcion) == descripcion.c_str()) && (c(&Transaccion::fkey_account_own) == own_account->id_account) && (c(&Transaccion::fkey_category) == categoria->id_categoria);

		trans = m_transaccionLB.exists(whereClause, &Transaccion::id_transaccion, &Transaccion::amount_colones, &Transaccion::amount_dolares, &Transaccion::fkey_account_own, &Transaccion::fkey_account_other, &Transaccion::line_date,
			&Transaccion::descripcion, &Transaccion::fkey_category, &Transaccion::fkey_concepto, &Transaccion::fkey_statement);
	}
	else
	{
		auto whereClause = (c(&Transaccion::fkey_concepto) == concepto->id_concepto) && (c(&Transaccion::line_date) == line_date) && (c(&Transaccion::fkey_statement) == statement->id_statement)
			&& (c(&Transaccion::amount_dolares) == dolares) && (c(&Transaccion::amount_colones) == colones)
			&& (c(&Transaccion::descripcion) == descripcion.c_str()) && (c(&Transaccion::fkey_account_own) == own_account->id_account) && (c(&Transaccion::fkey_category) == categoria->id_categoria);
	
		trans = m_transaccionLB.exists(whereClause, &Transaccion::id_transaccion, &Transaccion::amount_colones, &Transaccion::amount_dolares, &Transaccion::fkey_account_own, &Transaccion::line_date,
			&Transaccion::descripcion, &Transaccion::fkey_category, &Transaccion::fkey_concepto, &Transaccion::fkey_statement);
	}
#endif
	if( !trans)   // insert
	{
		trans = m_transaccionLB.insert(colones, dolares, own_account->id_account, account_other, line_date, descripcion, categoria->id_categoria, concepto->id_concepto, statement->id_statement, row);
		m_transaccionLB.insert_into_listbox(*trans);
		m_id.SetWindowTextW(JD::to_cstring(trans->id_transaccion));
	}
	else         // update
	{
		trans->line_date = line_date;
		trans->fkey_concepto = concepto->id_concepto;
		trans->descripcion = descripcion;
		trans->amount_dolares = dolares;
		trans->amount_colones = colones;
		trans->fkey_category = categoria->id_categoria;
		trans->fkey_account_other = account_other;
		trans->fkey_account_own = own_account->id_account;
		trans->fkey_statement = statement->id_statement;
		trans->row = row;
		m_transaccionLB.update(*trans);
	}
	m_id.SetWindowTextW(JD::to_cstring(trans->id_transaccion));
	m_transaccionLB.loadLB();
	m_trans = trans;
}

std::optional<Transaccion> TransaccionDlg::getCurrent()
{
	CString rId;
	m_id.GetWindowTextW(rId);

	if( rId.IsEmpty())
	{
		return std::nullopt;
	}
	auto s_id = JD::from_cstring(rId);
	auto id = stoi(s_id);
	
	using namespace sqlite_orm;

	auto& storage = Storage::getStorage();

	// auto trans = storage.get_pointer<Transaccion>(id);
	std::optional<Transaccion> trans = storage.get_optional<Transaccion>(id);
	return trans;
}


void TransaccionDlg::OnLbnSelchangeLTransaction()
{
	// TODO: Add your control notification handler code here
#if 0
	auto trans = getCurrent();
#endif
	using namespace sqlite_orm;

	auto& storage = Storage::getStorage();

#if 1	
	std::optional<Transaccion> trans = m_transaccionLB.current();
	if( ! trans)
	{
		MessageBoxW(L"Falta escoger transacción");
		return;
	}
	int cur_sel = m_transaccionLB.GetCurSel();
	using namespace date;
#endif

	auto rOleDateTime = JD::to_ole_date_time(trans->line_date);
	m_date_transaccion.SetCurSel(rOleDateTime);

	m_statementCB.select(trans->fkey_statement);
	m_other_accountCB.select(trans->fkey_account_other);
	m_conceptoCB.select(trans->fkey_concepto);
	m_own_accountCB.select(trans->fkey_account_own);
	m_categoriaCB.select(trans->fkey_category);
	Colones colones = trans->amount_colones;
	m_colones.SetWindowTextW(JD::to_cstring(colones));
	Dolares dolares = trans->amount_dolares;
	m_dolares.SetWindowTextW(JD::to_cstring(dolares));
	m_descripcion.SetWindowTextW(JD::to_cstring(trans->descripcion));
	m_id.SetWindowTextW(JD::to_cstring(trans->id_transaccion));

	//postMessage(m_listTransactions  );
	//m_transaccionLB.SetCurSel(cur_sel);

	//m_box.GetParent()->PostMessageW(WM_COMMAND, (WPARAM)MAKELONG(m_box.GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)(HWND)m_box.m_hWnd);
}

void TransaccionDlg::postMessage(CListBox& box)
{
	box.GetParent()->PostMessageW(WM_COMMAND, (WPARAM)MAKELONG(box.GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)(HWND)box.m_hWnd);
}



void TransaccionDlg::OnBnClickedBBorrar()
{
	// TODO: Add your control notification handler code here
#if 1
	auto trans = getCurrent();
#else
	auto trans = m_transaccionLB.current();
#endif
	if (!trans)
	{
		MessageBoxW(L"Falta escoger transaaccion");
		return;
	}
	bool has_links = RecordLinks::has_links(*trans);
	// ask to be sure!
	if (!has_links)
	{
		int index = m_transaccionLB.find_in_listbox(*trans);
		m_transaccionLB.SetCurSel(index);
		m_transaccionLB.delete_current_sel();
		m_id.SetWindowTextW(L"");
	}
}


void TransaccionDlg::OnBnClickedBUpdateTransaction()
{
#if 0
	// TODO: Add your control notification handler code here
#if 1
	auto trans = getCurrent();
#else
	auto trans = m_transaccionLB.current();
#endif
	if (!trans)
	{
		MessageBoxW(L"Falta escoger transaccion");
		return;
	}

	auto statement = m_statementCB.current();
	auto other_account = m_other_accountCB.current();
	auto concepto = m_conceptoCB.current();
	auto own_account = m_own_accountCB.current();
	auto category = m_categoriaCB.current();

	trans->fkey_statement = statement->id_statement;
	if (other_account)
	{
		trans->fkey_account_other = other_account->id_account;
	}
	trans->fkey_concepto = concepto->id_concepto;
	trans->fkey_account_own = own_account->id_account;
	trans->fkey_category = category->id_categoria;
	
	CString rDescripcion;
	m_descripcion.GetWindowTextW(rDescripcion);
	trans->descripcion = JD::from_cstring(rDescripcion);
	CString rColones;
	m_colones.GetWindowTextW(rColones);
	auto colones = JD::from_cstring(rColones);
	trans->amount_colones = JD::strip_to_long_double(colones);
	CString rDolares;
	m_dolares.GetWindowTextW(rDolares);
	auto dolares = JD::from_cstring(rDolares);
	trans->amount_dolares = JD::strip_to_long_double(dolares);

	COleDateTime rDateTime;
	m_date_transaccion.GetCurSel(rDateTime);
	trans->line_date = JD::to_sys_days(rDateTime);

	m_transaccionLB.update(*trans);
	m_transaccionLB.loadLB();

	m_trans = trans;
#endif
}
