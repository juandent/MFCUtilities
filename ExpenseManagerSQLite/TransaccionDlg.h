#pragma once
#include "BoxContents.h"

#include "Data_Tier.h"
// TransaccionDlg dialog

class TransaccionDlg : public CDialog
{
	DECLARE_DYNAMIC(TransaccionDlg)

	BoxContents<Transaccion, &Transaccion::id_transaccion> m_transaccionLB;
	BoxContents<Concepto, &Concepto::id_concepto, CComboBox> m_conceptoCB;
	BoxContents<Statement, &Statement::id_statement, CComboBox> m_statementCB;
	BoxContents<Account, &Account::id_account, CComboBox> m_own_accountCB;
	BoxContents<Account, &Account::id_account, CComboBox> m_other_accountCB;
	BoxContents<Categoria, &Categoria::id_categoria, CComboBox> m_categoriaCB;
	std::optional<Transaccion> m_trans;
public:
	TransaccionDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TransaccionDlg();


	void setDiscriminator( std::optional<Transaccion> trans)
	{
		m_trans = trans;
	}

	auto getCompleteObject() const
	{
		return m_trans;
	}
	
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TransaccionDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBAddStatement();
	afx_msg void OnBnClickedBAddConcept();
	afx_msg void OnBnClickedOk();
	std::optional<Transaccion> getCurrent();
	void postMessage(CListBox& box);
private:
	CMonthCalCtrl m_date_transaccion;
public:
	afx_msg void OnMcnSelchangeDateTransaction(NMHDR* pNMHDR, LRESULT* pResult);
private:
	CListBox m_listTransactions;
	CComboBox m_list_concepto;
	CComboBox m_list_statement;
	CComboBox m_list_own_accounts;
	CComboBox m_list_other_account;
	CComboBox m_list_category;
public:
	afx_msg void OnCbnSelchangeCEstadoCuenta();
	afx_msg void OnCbnSelchangeCOtherAccount();
	afx_msg void OnBnClickedBAplicarTransactions();
private:
	CEdit m_colones;
	CEdit m_dolares;
	CEdit m_descripcion;
	CEdit m_id;
public:
	afx_msg void OnLbnSelchangeLTransaction();
	afx_msg void OnBnClickedBBorrar();
	afx_msg void OnBnClickedBUpdateTransaction();
};
