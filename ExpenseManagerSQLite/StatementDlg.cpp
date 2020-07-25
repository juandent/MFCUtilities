// StatementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "StatementDlg.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(StatementDlg, CDialog)
	ON_BN_CLICKED(ID_B_APLICAR_STATEMENT, &StatementDlg::OnBnClickedBAplicarStatement)
END_MESSAGE_MAP()


// StatementDlg message handlers


BOOL StatementDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_statementLB.loadLB();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void StatementDlg::OnBnClickedBAplicarStatement()
{
	// TODO: Add your control notification handler code here
	using namespace date;
	
	COleDateTime rOleDateTime;
	m_transaction_date_picker.GetTime(rOleDateTime);
	int yearVal = rOleDateTime.GetYear();
	unsigned monthVal = rOleDateTime.GetMonth();
	unsigned dayVal = rOleDateTime.GetDay();

	year_month_day ymd{ year{yearVal}, month{monthVal}, day{dayVal} };
	sys_days statement_date = ymd;
	
	auto whereClause = (c(&Statement::date) == statement_date);

	std::optional<Statement> statement = m_statementLB.exists(whereClause, &Statement::id_statement, &Statement::date);

	if (!statement)	// new account
	{
		statement = m_statementLB.insert(statement_date);
		m_statementLB.insert_into_listbox(*statement);
	}

}
