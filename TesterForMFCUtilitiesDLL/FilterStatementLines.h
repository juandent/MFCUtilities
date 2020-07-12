#pragma once

#include "Model/Model.ORM.Definitions.h"
#include "ViewUtilities.h"

// FilterStatementLines dialog

class FilterStatementLines : public CDialogEx
{
	DECLARE_DYNAMIC(FilterStatementLines)

public:
	FilterStatementLines(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FilterStatementLines();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FilterStatementLines };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog() override;

	CComboBox m_categorias;
	CButton m_filter_categories;
	CComboBox m_conceptos;
	CComboBox m_statement_dates;
	CButton m_filter_date;

	CComboBox m_accounts;
	CButton m_filter_account;
	CComboBox m_owners;
	CButton m_filter_owner;
	CButton m_filter_concept;

	using selection_t = std::pair<bool, std::string>;

	selection_t m_cat;
	selection_t m_concepto;
	selection_t m_statement_date;
	selection_t m_account;
	selection_t m_owner;

	inline static selection_t s_cat;
	inline static selection_t s_concepto;
	inline static selection_t s_statement_date;
	inline static selection_t s_account;
	inline static selection_t s_owner;


public:
	afx_msg void OnBnClickedOk();


	auto get_where()
	{
		using namespace sqlite_orm;
		using namespace Model;
		using namespace ORM;
		using namespace std::string_literals;

		// TODO: Add your control notification handler code here

		auto category_where = c(&StatementLine::m_category_fid) == m_cat.second;
		auto concept_where = c(&StatementLine::m_concept_fid) == m_concepto.second;
		auto statement_date_where = c(&StatementLine::m_statement_date) == m_statement_date.second;
		auto account_where = c(&StatementLine::m_belongs_to_account_fid) == m_account.second;
		auto owner_where = c(&StatementLine::m_belongs_to_account_fid) == m_owner.second;

		auto where_clause = (not m_cat.first or category_where)
			and (not m_concepto.first or concept_where)
			and (not m_statement_date.first or statement_date_where)
			and (not m_account.first or account_where);

		return where_clause;
	}
};
