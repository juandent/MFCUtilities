#pragma once
#include "BoxContents.h"
#include "Data_Tier.h"

// StatementDlg dialog

class StatementDlg : public CDialog
{
	DECLARE_DYNAMIC(StatementDlg)
	BoxContents<Statement, &Statement::id_statement> m_statementLB;

	date::sys_days m_statement_date;
	std::optional<Statement> m_statement;
public:
	StatementDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~StatementDlg();


	void setDiscriminator( date::sys_days statement_date)
	{
		m_statement_date = statement_date;
	}

	auto getCompleteObject()
	{
		return m_statement;
	}
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_StatementDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CListBox m_list_statement;
	CDateTimeCtrl m_transaction_date_picker;
public:
	afx_msg void OnBnClickedBAplicarStatement();
	afx_msg void OnBnClickedBorrarStatement();
	afx_msg void OnLbnSelchangeLEstadosDeCuenta();
	afx_msg void OnBnClickedBUpdateStatement();
};
