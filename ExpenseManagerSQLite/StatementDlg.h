#pragma once
#include "BoxContents.h"
#include "Data_Tier.h"

// StatementDlg dialog

class StatementDlg : public CDialog
{
	DECLARE_DYNAMIC(StatementDlg)
	BoxContents<Statement, &Statement::id_statement> m_statementLB;
public:
	            StatementDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~StatementDlg();

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
};
