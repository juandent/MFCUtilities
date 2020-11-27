// ConceptsAndAccounts.cpp : implementation file
//

#include "stdafx.h"
#include "Data_Tier.h"

#include "ExpenseManagerSQLite.h"
#include "ConceptsAndAccounts.h"

// #include "Data_Tier.h"

#include <afxext.h>

#include "BoxContents.h"
#include "CompoundStatementLine.h"
// #include "StatementLineAdapter.h"
// #include "RecordLinks.h"


// ConceptsAndAccounts

IMPLEMENT_DYNCREATE(ConceptsAndAccounts, CFormView)

ConceptsAndAccounts::ConceptsAndAccounts()
	: CFormView(IDD_ConceptsAndAccounts),
	m_grid_controller(m_statementLines, 11),
	m_paisLB{ m_paises, [](Pais& pais)
{
	auto display = JD::to_cstring(pais.name);
	return display;
} },
m_duenoLB{ m_duenos,
[](AccountOwner& owner)
		{
			auto display = JD::to_cstring(owner.name);
			return display;
		} },
	m_bancoLB{ m_bancos,
	[](Banco& banco)
		{
			auto display = JD::to_cstring(banco.nombre + " - " + banco.ubicacion);
			return display;
		} },
			m_accountLB{ m_cuentas,
			[](Account& account)
				{
					auto display = JD::to_cstring(account.number + " - " + account.description);
					return display;
				} },
			m_conceptoLB{ m_conceptos,
			[](Concepto& concepto)
			{
					auto display = JD::to_cstring(concepto.name);
					return display;
			}},
			m_transaccionesLB { m_transacciones, [](Transaccion& trans)
			{
				Colones  colones = trans.amount_colones;
				Dolares dolares = trans.amount_dolares;
#if 0
				return JD::to_cstring(trans.id_transaccion) + L" - #" + JD::to_cstring(trans.row) + L" - " +
					JD::to_cstring(trans.line_date) + L" - " + JD::to_cstring(trans.descripcion) + L" - " + JD::to_cstring(colones) + L" - " + JD::to_cstring(dolares);
#else
				return JD::to_cstring(trans.line_date) + L" - #" + JD::to_cstring(trans.row) + L" : " + JD::to_cstring(trans.id_transaccion) + L" - " 
					 + JD::to_cstring(trans.descripcion) + L" - " + JD::to_cstring(colones) + L" - " + JD::to_cstring(dolares);
#endif
			}},
			m_statementsLB {  m_statement_list, [](Statement& statement)
			{
				return JD::to_cstring(statement.id_statement) + L" - " + JD::to_cstring(statement.date);
			}}
{
}

ConceptsAndAccounts::~ConceptsAndAccounts()
{
}

void ConceptsAndAccounts::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_statementLines);
	DDX_Control(pDX, IDC_E_ACCOUNT_NUMBER, m_account_number);
	DDX_Control(pDX, IDC_E_ACCOUNT_DESC, m_account_description);
	DDX_Control(pDX, IDC_E_DUENO, m_dueno);
	DDX_Control(pDX, IDC_E_BANCO, m_banco);
	DDX_Control(pDX, IDC_E_PAIS, m_pais);
	DDX_Control(pDX, IDC_C_IS_TARJETA, m_is_tarjeta);
	DDX_Control(pDX, IDC_L_PAISES, m_paises);
	DDX_Control(pDX, IDC_L_DUENOS, m_duenos);
	DDX_Control(pDX, IDC_L_BANCOS, m_bancos);
	DDX_Control(pDX, IDC_L_CUENTAS, m_cuentas);
	//	DDX_Control(pDX, IDC_L_EXPANDED_ACCOUNTS, m_expanded_accounts);
	DDX_Control(pDX, IDC_E_UBICACION, m_ubicacion);
	DDX_Control(pDX, IDC_E_CONCEPTO, m_concepto);
	//DDX_Control(pDX, IDC_E_CUENTA_DEST, m_cuenta_destino);
	//DDX_Control(pDX, IDC_E_CUENTA_PROPIA, m_cuenta_origen);
	DDX_Control(pDX, IDC_E_DESCRIPCION, m_descripcion_linea);
	DDX_Control(pDX, IDC_E_LINE_DATE, m_line_date);
	DDX_Control(pDX, IDC_E_STMT_DATE, m_stmt_date);
	DDX_Control(pDX, IDC_E_AMOUNT_LOCAL, m_amount_local);
	DDX_Control(pDX, IDC_E_AMOUNT_DOLARES, m_amount_dolares);
	DDX_Control(pDX, IDC_E_CUENTA_PROPIA, m_cuenta_propia);
	DDX_Control(pDX, IDC_E_CUENTA_OTRA, m_otra_cuenta);
	//	DDX_Control(pDX, IDC_L_CUENTAS_MIAS, m_cuentas_propias);
	DDX_Control(pDX, IDC_L_TRANSACTIONS, m_transacciones);
	DDX_Control(pDX, IDC_L_CONCEPTOS, m_conceptos);
	DDX_Control(pDX, IDC_L_STATEMENTS, m_statement_list);
}

BEGIN_MESSAGE_MAP(ConceptsAndAccounts, CFormView)

	ON_WM_CREATE()
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_BN_CLICKED(IDC_B_LOAD_COMPOUND_DOC, &ConceptsAndAccounts::OnBnClickedBLoadCompoundDoc)
	ON_BN_CLICKED(IDC_B_PAIS_ADD, &ConceptsAndAccounts::OnBnClickedBPaisAdd)
	ON_BN_CLICKED(IDC_B_BANCO_ADD, &ConceptsAndAccounts::OnBnClickedBBancoAdd)
	ON_BN_CLICKED(IDC_B_DUENO_ADD, &ConceptsAndAccounts::OnBnClickedBDuenoAdd)
	ON_BN_CLICKED(IDC_B_CUENTA_ADD, &ConceptsAndAccounts::OnBnClickedBCuentaAdd)
	ON_LBN_SELCHANGE(IDC_L_CUENTAS, &ConceptsAndAccounts::OnLbnSelchangeLCuentas)
	ON_LBN_SELCHANGE(IDC_L_BANCOS, &ConceptsAndAccounts::OnLbnSelchangeLBancos)
	ON_BN_CLICKED(IDC_B_READ_STATEMENTLINE, &ConceptsAndAccounts::OnBnClickedBReadStatementline)
	ON_LBN_SELCHANGE(IDC_L_DUENOS, &ConceptsAndAccounts::OnLbnSelchangeLDuenos)
	ON_BN_CLICKED(IDC_B_DESELECT_ACCOUNTS, &ConceptsAndAccounts::OnBnClickedBDeselectAccounts)
	ON_BN_CLICKED(IDC_B_DESELECT_DUENOS, &ConceptsAndAccounts::OnBnClickedBDeselectDuenos)
	ON_BN_CLICKED(IDC_B_DESELECT_BANCOS, &ConceptsAndAccounts::OnBnClickedBDeselectBancos)
	ON_BN_CLICKED(IDC_B_DESELECT_PAIS, &ConceptsAndAccounts::OnBnClickedBDeselectPais)
	ON_BN_CLICKED(IDC_B_CONCEPTO, &ConceptsAndAccounts::OnBnClickedBConcepto)
	ON_LBN_SELCHANGE(IDC_L_CONCEPTOS, &ConceptsAndAccounts::OnLbnSelchangeLConceptos)
	ON_BN_CLICKED(IDC_B_DESELECT_CONCEPTOS, &ConceptsAndAccounts::OnBnClickedBDeselectConceptos)
	ON_LBN_SELCHANGE(IDC_L_TRANSACTIONS, &ConceptsAndAccounts::OnLbnSelchangeLTransactions)
	ON_BN_CLICKED(IDC_B_SAVE_TO_DB, &ConceptsAndAccounts::OnBnClickedBSaveToDb)
	ON_LBN_SELCHANGE(IDC_L_STATEMENTS, &ConceptsAndAccounts::OnLbnSelchangeLStatements)
	ON_BN_CLICKED(IDC_B_REMOVE_STATEMENT, &ConceptsAndAccounts::OnBnClickedBRemoveStatement)
	ON_BN_CLICKED(IDC_B_REMOVE_ACCOUNT, &ConceptsAndAccounts::OnBnClickedBRemoveAccount)
	ON_BN_CLICKED(IDC_B_REMOVE_TRANSACTION, &ConceptsAndAccounts::OnBnClickedBRemoveTransaction)
END_MESSAGE_MAP()


// ConceptsAndAccounts diagnostics

#ifdef _DEBUG
void ConceptsAndAccounts::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ConceptsAndAccounts::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG



// ConceptsAndAccounts message handlers

void ConceptsAndAccounts::OnGridClick(NMHDR* pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	if (pItem->hdr.idFrom == IDC_GRID)
	{
		if( pItem->iRow > 0)
		{
			m_row = pItem->iRow;
			SaveRowToDb();
		}

		
		// m_grid_controller.OnGridClick(pNotifyStruct);
		//m_controller->toggleEnableForLine(pItem->iRow, pItem->iColumn);
		//m_statementLinesController->toggleEnableForLine(pItem->iRow, pItem->iColumn);
	}
}



int ConceptsAndAccounts::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

///	m_grid_controller.OnInitialUpdate();
///	m_banco.SetWindowTextW(L"BANCO");
	
	return 0;
}


void ConceptsAndAccounts::OnBnClickedBLoadCompoundDoc()
{
	// TODO: Add your control notification handler code here
	auto startPath = fs::current_path();
	auto current = startPath.string();
	auto currPath = JD::to_cstring(current);
	CFileDialog dlg{ true };
	auto ret = dlg.DoModal();
	if (ret != 1)	return;
	auto folderPath = dlg.GetPathName();
	auto selectedFileName = JD::from_cstring(folderPath);
	LoadFile(selectedFileName);
}


void ConceptsAndAccounts::LoadFile(const std::string& fileName)
{
	m_file.load(fileName);
	bool isFirstLine = true;
	for( auto it = m_file.cbegin(); it != m_file.cend(); ++it)
	{
		if( isFirstLine)
		{
			isFirstLine = false;
			continue;
		}
		auto x = *it;

		Translation t;
		t.statement_date = JD::to_date(x[0]);
		t.origin_account = x[1];
		t.line_date = JD::to_date(x[2]);
		t.concepto = x[3];
		t.amount_local = std::stod(x[4]);
		t.amount_dollars = std::stod(x[5]);
		t.is_enabled = x[6] == "1";
		t.category_name = x[7];
		t.description = x[8];
		if( x.size() > 9)
			t.owner_name = x[9];

		//StatementLineAdapter adapt(t);
		
		m_grid_controller.lines.push_back(t);
	}
	m_grid_controller.OnInitialUpdate();
}



void ConceptsAndAccounts::OnBnClickedBPaisAdd()
{
	// TODO: Add your control notification handler code here
	CString rStr;
	m_pais.GetWindowTextW(rStr);

	if (rStr.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre del pais");
		return;
	}

	auto name = JD::from_cstring(rStr);
	auto whereClause = c(&Pais::name) == name.c_str();
	
	std::optional<Pais> pais = m_paisLB.exists(whereClause, &Pais::id_pais, &Pais::name);

	if( ! pais)
	{
		pais = m_paisLB.insert(name);
		m_paisLB.insert_into_listbox(*pais);
	}
}



// banco --> nombre, ubicacion

void ConceptsAndAccounts::OnBnClickedBBancoAdd()
{
	// TODO: Add your control notification handler code here
	CString rBanco, rUbicacion;
	m_banco.GetWindowTextW(rBanco);
	m_ubicacion.GetWindowTextW(rUbicacion);

	if (rBanco.IsEmpty() || rUbicacion.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre del banco o su ubicacion");
		return;
	};

	
	auto name = JD::from_cstring(rBanco);
	auto ubicacion = JD::from_cstring(rUbicacion);

	auto whereClause = (c(&Banco::nombre) == name.c_str()) && (c(&Banco::ubicacion) == ubicacion.c_str());

	std::optional<Banco> banco = m_bancoLB.exists(whereClause, &Banco::id_bank, &Banco::nombre);

	if (!banco)
	{
		// need to get current pais
		std::optional<Pais> pais = m_paisLB.current();
		if (pais)
		{
			banco->fkey_pais = pais->id_pais;
			banco = m_bancoLB.insert(name, ubicacion);
			m_bancoLB.insert_into_listbox(*banco);
		}
	}


	//m_bancos.AddString(rStr);
}


void ConceptsAndAccounts::OnBnClickedBDuenoAdd()
{
	// TODO: Add your control notification handler code here
	CString rStr;
	m_dueno.GetWindowTextW(rStr);

	if (rStr.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre del dueño");
		return;
	}

	auto name = JD::from_cstring(rStr);
	auto whereClause = (c(&AccountOwner::name) == name.c_str());

	std::optional<AccountOwner> dueno = m_duenoLB.exists(whereClause, &AccountOwner::id_owner, &AccountOwner::name);

	if (!dueno)
	{
		dueno = m_duenoLB.insert(name);
		m_duenoLB.insert_into_listbox(*dueno);
	}

//	m_duenos.AddString(rStr);
}


void ConceptsAndAccounts::OnBnClickedBCuentaAdd()
{
	// TODO: Add your control notification handler code here
	CString cs_numero, cs_descripcion;
	bool isTarjeta;

	m_account_number.GetWindowTextW(cs_numero);
	m_account_description.GetWindowTextW(cs_descripcion);

	if(cs_numero.IsEmpty() || cs_descripcion.IsEmpty())
	{
		MessageBox(L"Falta el numero o descripcion de la cuenta");
		return;
	}

	auto numero = JD::from_cstring(cs_numero);
	auto descripcion = JD::from_cstring(cs_descripcion);
	
	isTarjeta = m_is_tarjeta.GetCheck();

	auto banco = m_bancoLB.current();

	if( ! banco)
	{
		MessageBoxW(L"Falta escoger banco");
		return;
	}


	auto owner = m_duenoLB.current();

	if( ! owner)
	{
		MessageBoxW(L"Falta escoger dueño");
		return;

	}

	auto whereClause = (c(&Account::number) == numero.c_str());

	std::optional<Account> cuenta = m_accountLB.exists(whereClause, &Account::id_account, &Account::number);

	if (!cuenta)
	{
		cuenta = m_accountLB.insert(numero,banco->id_bank, owner->id_owner, descripcion, isTarjeta);
		m_accountLB.insert_into_listbox(*cuenta);
	}


	//m_cuentas.AddString(numero);
	//m_expanded_accounts.AddString(cuenta);
}


void ConceptsAndAccounts::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	refresh();
}

void ConceptsAndAccounts::refresh()
{
	m_paisLB.loadLB();
	m_duenoLB.loadLB();
	m_accountLB.loadLB();
	m_bancoLB.loadLB();
	m_conceptoLB.loadLB();
	m_transaccionesLB.loadLB();
	m_statementsLB.loadLB();
}

void ConceptsAndAccounts::OnLbnSelchangeLCuentas()
{
	// TODO: Add your control notification handler code here
	std::optional<Account> act = m_accountLB.current();
	if( act)
	{
		std::optional<AccountOwner> owner = m_duenoLB.select(act->fkey_account_owner);
		std::optional<Banco> banco = m_bancoLB.select(act->fkey_bank);
	
	}
}


void ConceptsAndAccounts::OnLbnSelchangeLBancos()
{
	// TODO: Add your control notification handler code here
	std::optional<Banco> banco = m_bancoLB.current();
	if( banco )
	{
		std::optional<Pais> pais = m_paisLB.select(banco->fkey_pais);
	}
}


void ConceptsAndAccounts::OnLbnSelchangeLDuenos()
{
	// TODO: Add your control notification handler code here
	std::optional<AccountOwner> owner = m_duenoLB.current();
	if(owner)
	{
	//	std::optional<AccountOwner> 
	}

}

void ConceptsAndAccounts::OnLbnSelchangeLConceptos()
{
	// TODO: Add your control notification handler code here
	auto concepto = m_conceptoLB.current();
	if( concepto)
	{
		std::optional<Account> cuenta = m_accountLB.select(concepto->fkey_account);
	}
}



void ConceptsAndAccounts::OnBnClickedBReadStatementline()
{
	// TODO: Add your control notification handler code here
	auto range = m_statementLines.GetSelectedCellRange();
	int row = range.GetMaxRow();

	if (row == -1)	return;

	CString amount_local = m_statementLines.GetItemText(row, StatementLineGridController::Columns::AMOUNT_LOCAL);
	m_amount_local.SetWindowTextW(amount_local);

	CString amount_dolares = m_statementLines.GetItemText(row, StatementLineGridController::Columns::AMOUNT_DOLLARS);
	m_amount_dolares.SetWindowTextW(amount_dolares);

	// bool isNegativeAmount = isNegative(amount_local) || isNegative(amount_dolares);
	
	CString own_account = m_statementLines.GetItemText(row, StatementLineGridController::Columns::OWN_ACCOUNT);
	m_cuenta_propia.SetWindowTextW(own_account);

	CString concepto = m_statementLines.GetItemText(row, StatementLineGridController::Columns::CONCEPTO);
	m_concepto.SetWindowTextW(concepto);

	CString line_date = m_statementLines.GetItemText(row, StatementLineGridController::Columns::LINE_DATE);
	m_line_date.SetWindowTextW(line_date);

	CString stmt_date = m_statementLines.GetItemText(row, StatementLineGridController::Columns::STMT_DATE);
	m_stmt_date.SetWindowTextW(stmt_date);

	CString descrip = m_statementLines.GetItemText(row, StatementLineGridController::Columns::DESCRIPCION);
	m_descripcion_linea.SetWindowTextW(descrip);	
}

std::string ConceptsAndAccounts::readCell(const int row, const int column)
{
	return JD::from_cstring(m_statementLines.GetItemText(row, column));
}

bool ConceptsAndAccounts::verifyRange(const int row)
{
	bool ok = row >= 1 && row < m_statementLines.GetRowCount();
	if (!ok) {
		throw std::exception{ "Row fuera de rango" };
	}
	return true;
}

////////////////////////////////////////////////
///
///
double ConceptsAndAccounts::getAmountLocal(const int row)
{
	verifyRange(row);
	auto val = readCell(row, StatementLineGridController::Columns::AMOUNT_LOCAL);
	return JD::strip_to_long_double(val);
}
double ConceptsAndAccounts::getAmountDolares(const int row)
{
	verifyRange(row);
	auto val = readCell(row, StatementLineGridController::Columns::AMOUNT_DOLLARS);
	return JD::strip_to_long_double(val);
}
std::string ConceptsAndAccounts::getOwnAccountNumber(const int row)
{
	verifyRange(row);
	auto val = readCell(row, StatementLineGridController::Columns::OWN_ACCOUNT);
	return val;
}

std::string ConceptsAndAccounts::getConceptoName(const int row)
{
	verifyRange(row);
	auto val = readCell(row, StatementLineGridController::Columns::CONCEPTO);
	return val;
}

date::sys_days ConceptsAndAccounts::getLineDate(const int row)
{
	verifyRange(row);
	auto val = readCell(row, StatementLineGridController::Columns::LINE_DATE);
	return JD::to_date(val);
}

date::sys_days ConceptsAndAccounts::getStatementDate(const int row)
{
	verifyRange(row);
	auto val = readCell(row, StatementLineGridController::Columns::STMT_DATE);
	return JD::to_date(val);
}

std::string ConceptsAndAccounts::getDescripcion(const int row)
{
	verifyRange(row);
	auto val = readCell(row, StatementLineGridController::Columns::DESCRIPCION);
	return val;
}

std::string ConceptsAndAccounts::getCategoryName(const int row)
{
	verifyRange(row);
	auto val = readCell(row, StatementLineGridController::Columns::CATEGORY_NAME);
	return val;
}
/// <summary>
/// ////////////////////////////////
/// </summary>

void ConceptsAndAccounts::OnBnClickedBDeselectAccounts()
{
	// TODO: Add your control notification handler code here
	m_cuentas.SetCurSel(-1);
}


void ConceptsAndAccounts::OnBnClickedBDeselectDuenos()
{
	// TODO: Add your control notification handler code here
	m_duenos.SetCurSel(-1);
}


void ConceptsAndAccounts::OnBnClickedBDeselectBancos()
{
	// TODO: Add your control notification handler code here
	m_bancos.SetCurSel(-1);
}


void ConceptsAndAccounts::OnBnClickedBDeselectPais()
{
	// TODO: Add your control notification handler code here
	m_paises.SetCurSel(-1);
}


void ConceptsAndAccounts::OnBnClickedBDeselectConceptos()
{
	// TODO: Add your control notification handler code here
	m_conceptos.SetCurSel(-1);
}


void ConceptsAndAccounts::OnBnClickedBConcepto()
{
	// TODO: Add your control notification handler code here
	// we must have an account selected
	CString cs_concepto_name;
	m_concepto.GetWindowTextW(cs_concepto_name);

	if( cs_concepto_name.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre del concepto");
		return;
	}

	auto concepto_name = JD::from_cstring(cs_concepto_name);

	auto cuenta = m_accountLB.current();
	if (!cuenta)
	{
		MessageBoxW(L"Falta escoger la cuenta");
		return;
	}

	auto whereClause = (c(&Concepto::name) == concepto_name.c_str());

	std::optional<Concepto> concepto = m_conceptoLB.exists(whereClause, &Concepto::id_concepto, &Concepto::name);

	if (!concepto)
	{
		concepto = m_conceptoLB.insert(concepto_name, cuenta->id_account);
		m_conceptoLB.insert_into_listbox(*concepto);
	}

}




void ConceptsAndAccounts::OnLbnSelchangeLTransactions()
{
	// TODO: Add your control notification handler code here
	std::optional<Transaccion> trans = m_transaccionesLB.current();
	if (trans)
	{
		std::optional<Account> acct = m_accountLB.select(*trans->fkey_account_other);
		auto statement = m_statementsLB.select(trans->fkey_statement);
	}

}

void ScrollDownNRows(CGridCtrl& grid, int num_rows)
{
	for (int row = 0; row < num_rows; ++row)
	{
		grid.SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	}
}

void ScrollUpNRows(CGridCtrl& grid, int num_rows)
{
	for (int row = 0; row < num_rows; ++row)
	{
		grid.SendMessage(WM_VSCROLL, SB_LINEUP, 0);
	}
}

bool ConceptsAndAccounts::SaveRowToDb()
{
	m_statementLines.SetSelectedRange(m_row, 0, m_row, 10);

	CompoundStatementLine comp{ this };
	auto r1 = comp.set_own_account(&ConceptsAndAccounts::getOwnAccountNumber);
	if (r1)
	{
		auto r2 = comp.set_concepto(&ConceptsAndAccounts::getConceptoName);
		if (r2)
		{
			auto r3 = comp.setStatement(&ConceptsAndAccounts::getStatementDate);
			if (r3)
			{
				auto r4 = comp.set_category(&ConceptsAndAccounts::getCategoryName);
				if (r4)
				{
					auto r5 = comp.set_transaction(&ConceptsAndAccounts::getAmountLocal, &ConceptsAndAccounts::getAmountDolares, &ConceptsAndAccounts::getLineDate, &ConceptsAndAccounts::getDescripcion);
					return r5.second;
				}
			}
		}
	}
	return false;
}

void ConceptsAndAccounts::OnBnClickedBSaveToDb()
{
	// TODO: Add your control notification handler code here

	const auto row_count = m_statementLines.GetRowCount();
	for (m_row = 1; m_row < row_count ; ++m_row)
	{
		if (!SaveRowToDb())
			break;
		// int ret = MessageBoxW(L"Desea continuar?", L"Confirme", MB_CANCELTRYCONTINUE);
		// if (ret == 2)
		// 	break;
		// ;
		ScrollDownNRows(m_statementLines, 1);
	}
	refresh();
}


void ConceptsAndAccounts::OnLbnSelchangeLStatements()
{
	// TODO: Add your control notification handler code here
	auto statement = m_statementsLB.current();
	//m_transaccionesLB.select(statement->)
}


void ConceptsAndAccounts::OnBnClickedBRemoveStatement()
{
	// TODO: Add your control notification handler code here
	m_statementsLB.delete_current_sel();
}


void ConceptsAndAccounts::OnBnClickedBRemoveAccount()
{
	// TODO: Add your control notification handler code here
	m_accountLB.delete_current_sel();
}


void ConceptsAndAccounts::OnBnClickedBRemoveTransaction()
{
	// TODO: Add your control notification handler code here
	m_transaccionesLB.delete_current_sel();
}
