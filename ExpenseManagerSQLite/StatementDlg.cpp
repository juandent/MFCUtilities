// StatementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "StatementDlg.h"
#include "afxdialogex.h"
#include "RecordLinks.h"


// StatementDlg dialog

IMPLEMENT_DYNAMIC(StatementDlg, CDialog)

StatementDlg::StatementDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_StatementDlg, pParent),
	m_statementLB{ m_list_statement, [](Statement& statement)
	{
		return JD::to_cstring(statement.date);
	}}
{

}

StatementDlg::~StatementDlg()
{
}

void StatementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_L_ESTADOS_DE_CUENTA, m_list_statement);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STATEMENT, m_transaction_date_picker);
	DDX_Control(pDX, IDC_E_ID_STATEMENT, m_id_statement);
}


BEGIN_MESSAGE_MAP(StatementDlg, CDialog)
	ON_BN_CLICKED(ID_B_APLICAR_STATEMENT, &StatementDlg::OnBnClickedBAplicarStatement)
	ON_BN_CLICKED(ID_BORRAR_STATEMENT, &StatementDlg::OnBnClickedBorrarStatement)
	ON_LBN_SELCHANGE(IDC_L_ESTADOS_DE_CUENTA, &StatementDlg::OnLbnSelchangeLEstadosDeCuenta)
	ON_BN_CLICKED(IDC_B_UPDATE_STATEMENT, &StatementDlg::OnBnClickedBUpdateStatement)
END_MESSAGE_MAP()


// StatementDlg message handlers


BOOL StatementDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_statementLB.loadLB();
	
	//m_statement = m_statementLB.current();
	if (m_statement)
	{
		auto date = JD::to_ole_date_time(m_statement_date);
		m_transaction_date_picker.SetTime(date);
		setIdFromRecord<Statement>(m_id_statement, m_statement->id_statement);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void StatementDlg::OnBnClickedBAplicarStatement()
{
	// TODO: Add your control notification handler code here
	auto statement = getCurrent<Statement>(m_id_statement);
	
	using namespace date;
	
	COleDateTime rOleDateTime;
	m_transaction_date_picker.GetTime(rOleDateTime);

	sys_days statement_date = JD::to_sys_days(rOleDateTime);

	if (!statement)
	{
		auto whereClause = (c(&Statement::date) == statement_date);
		std::optional<Statement> statement_by_value = m_statementLB.exists(whereClause, &Statement::id_statement, &Statement::date);
		if (statement_by_value)
		{
			MessageBoxW(L"Estado de cuenta found for another primary key");
		}
		statement = statement_by_value;

	}
	if (!statement)	// new account
	{
		statement = m_statementLB.insert(statement_date);
		m_statementLB.insert_into_listbox(*statement);
	}
	else     // update
	{
		statement->date = statement_date;
		m_statementLB.update(*statement);
	}
	m_statementLB.loadLB();
	m_statement = statement;
	setIdFromRecord<Statement>(m_id_statement, statement->id_statement);
}


void StatementDlg::OnBnClickedBorrarStatement()
{
	// TODO: Add your control notification handler code here
	if (m_statementLB.delete_current_sel())
	{
		setIdFromRecord<Statement>(m_id_statement, -1);
	}
}


void StatementDlg::OnLbnSelchangeLEstadosDeCuenta()
{
	// TODO: Add your control notification handler code here
	using namespace date;

	auto statement = m_statementLB.current();
	if( ! statement)
	{
		MessageBoxW(L"Falta escoger estado de cuenta");
		return;
	}

	auto fecha = statement->date;

	COleDateTime rDateTime = JD::to_ole_date_time(fecha);
	
	m_transaction_date_picker.SetTime(rDateTime);
	setIdFromRecord<Statement>(m_id_statement, statement->id_statement);

}


void StatementDlg::OnBnClickedBUpdateStatement()
{
#if 0
	// TODO: Add your control notification handler code here
	using namespace date;

	auto statement = m_statementLB.current();
	if (!statement)
	{
		MessageBoxW(L"Falta escoger estado de cuenta");
		return;
	}

	COleDateTime rDateTime;
	m_transaction_date_picker.GetTime(rDateTime);
	auto fecha = JD::to_sys_days(rDateTime);
	if( statement->date == fecha)
	{
		MessageBox(L"No hay cambios en este objeto");
		return;
	}

	statement->date = fecha;
	m_statementLB.update(*statement);
	m_statementLB.loadLB();
	
	m_statement = statement;
#endif
}
