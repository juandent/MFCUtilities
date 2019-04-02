#pragma once


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
};
