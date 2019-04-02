// FilterStatementLines.cpp : implementation file
//

#include "stdafx.h"
#include "TesterForMFCUtilitiesDLL.h"
#include "FilterStatementLines.h"
#include "afxdialogex.h"
#include "../ORM/ORM.Repository.h"
#include "ModelToView.h"

// FilterStatementLines dialog

IMPLEMENT_DYNAMIC(FilterStatementLines, CDialogEx)

FilterStatementLines::FilterStatementLines(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FilterStatementLines, pParent)
{

}

FilterStatementLines::~FilterStatementLines()
{
}

void FilterStatementLines::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CATEGORIAS, m_categorias);
	DDX_Control(pDX, IDC_CHECK_FILTER_CATEGORIA2, m_filter_categories);
	DDX_Control(pDX, IDC_COMBO_CONCEPTOS, m_conceptos);
	DDX_Control(pDX, IDC_COMBO_STATEMENT_DATES, m_statement_dates);
	DDX_Control(pDX, IDC_CHECK_FILTER_STATEMENT_DATE, m_filter_date);
	DDX_Control(pDX, IDC_COMBO_ACCOUNTS, m_accounts);
	DDX_Control(pDX, IDC_CHECK_FILTER_ACCOUNT, m_filter_account);
	DDX_Control(pDX, IDC_COMBO_OWNERS, m_owners);
	DDX_Control(pDX, IDC_CHECK_FILTER_OWNER, m_filter_owner);
}


BEGIN_MESSAGE_MAP(FilterStatementLines, CDialogEx)
END_MESSAGE_MAP()


// FilterStatementLines message handlers



BOOL FilterStatementLines::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	FillAll(&Model::Category::m_name_id, m_categorias);
	FillAll(&Model::Concept::m_concept_id, m_conceptos);
	FillAll(&Model::Statement::m_statementDate, m_statement_dates);
	FillAll(&Model::Account::m_number_id, m_accounts);
	FillAll(&Model::Person::m_last_name, m_owners);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
