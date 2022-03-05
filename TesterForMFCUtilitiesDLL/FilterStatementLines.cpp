// FilterStatementLines.cpp : implementation file
//

#include "stdafx.h"
#include "TesterForMFCUtilitiesDLL.h"
#include "FilterStatementLines.h"
#include "afxdialogex.h"
#include "../ORM/ORM.Repository.h"
#include "ModelToView.h"
#include "ViewUtilities.h"

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
	DDX_Control(pDX, IDC_CHECK_FILTER_CONCEPTOS, m_filter_concept);
}


BEGIN_MESSAGE_MAP(FilterStatementLines, CDialogEx)
	ON_BN_CLICKED(IDOK, &FilterStatementLines::OnBnClickedOk)
END_MESSAGE_MAP()


// FilterStatementLines message handlers




template<typename Entity, typename ResultSet,  typename TypeToDisplay, typename Widget>
void fill(ResultSet results, TypeToDisplay Entity::* pData, Widget& widget, std::function<CString(std::string)> func)
{
	for (auto& element : results)
	{
		widget.AddString(func(element.*pData));
	}
}

namespace as_text
{
	CString from_string( const std::string& str)
	{
		return ::JD::to_cstring(str);
	}
	
}


// template<typename Entity, typename TypeToDisplay>
// void FillAllWithData(TypeToDisplay Entity::* pData, CComboBox& combo)
// {
// 	auto resultset = ORM::storage.get_all<Entity>();
// 	for (auto& element : resultset)
// 	{
// 		combo.AddString(JD::to_cstring(element.*pData));
// 	}
// }

template<typename Entity, typename TypeToDisplay>
void operator<<( CComboBox& combo, TypeToDisplay Entity::* pData)
{
		auto resultset = ORM::storage.get_all<Entity>();
		for (auto& element : resultset)
		{
			combo.AddString(JD::to_cstring(element.*pData));
		}
}



BOOL FilterStatementLines::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	using namespace sqlite_orm;


	//fill(ORM::storage.get_all<Model::Category>(), &Model::Category::m_name_id, m_categorias, as_text::from_string);

	m_categorias << &Model::Category::m_name_id;
	
	// TODO:  Add extra initialization here
	//FillAllWithData(&Model::Category::m_name_id, m_categorias);
	FillAllWithData(&Model::Concept::m_concept_id, m_conceptos);
	FillAllWithData(&Model::Statement::m_statementDate, m_statement_dates);
	FillAllWithData(&Model::Account::m_number_id, m_accounts);
	FillAllWithFunc(&Model::Person::get_full_name, m_owners);

	// if( s_cat.first )
	// {
	// 	CString str = JD::to_cstring(s_cat.second);
	// 	m_categorias.SelectString(0, str);
	// 	m_filter_categories.SetCheck(true);
	// }
	set_selection(s_cat, m_filter_categories, m_categorias);
	set_selection(s_concepto, m_filter_concept, m_conceptos);
	set_selection(s_statement_date, m_filter_date, m_statement_dates);
	set_selection(s_account, m_filter_account, m_accounts);
	set_selection(s_owner, m_filter_owner, m_owners);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


namespace std
{
	template<>
	struct less<Model::StatementLine>
	{
		bool operator()(const Model::StatementLine& lhs, const Model::StatementLine& rhs)
		{
			return lhs.m_id < rhs.m_id;
		}
	};
}

void FilterStatementLines::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_cat = get_selection(m_filter_categories, m_categorias);
	m_concepto = get_selection(m_filter_concept, m_conceptos);
	m_statement_date = get_selection(m_filter_date, m_statement_dates);
	m_account = get_selection(m_filter_account, m_accounts);
	m_owner = get_selection(m_filter_owner, m_owners);

	s_cat = m_cat;
	s_concepto = m_concepto;
	s_statement_date = m_statement_date;
	s_account = m_account;
	s_owner = m_owner;

	CDialogEx::OnOK();
}
