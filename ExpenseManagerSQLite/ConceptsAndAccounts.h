#pragma once
#include "CSVFile.h"
#include "Data_Tier.h"
#include "ListboxContents.h"
#include "StatementLineGridController.h"


// ConceptsAndAccounts form view

class ConceptsAndAccounts : public CFormView
{
	DECLARE_DYNCREATE(ConceptsAndAccounts)

	ListboxContents<Pais, &Pais::id_pais> m_paisLB;
	ListboxContents<AccountOwner, &AccountOwner::id_owner> m_duenoLB;
	ListboxContents<Banco, &Banco::id_bank> m_bancoLB;
	ListboxContents<Account, &Account::id_account> m_accountLB;
	ListboxContents<Concepto, &Concepto::id_concepto> m_conceptoLB;
protected:
	ConceptsAndAccounts();           // protected constructor used by dynamic creation
	virtual ~ConceptsAndAccounts();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ConceptsAndAccounts };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CJDGridCtrl m_statementLines;
	StatementLineGridController m_grid_controller;
	CSVFile m_file;
	void LoadFile(const std::string& fileName);
public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedBLoadCompoundDoc();
private:
//	CListBox m_listOfAccounts;
//	CEdit m_account;
public:
//	afx_msg void OnBnClickedInsertAccount();
private:
	CEdit m_account_number;
	CEdit m_account_description;
	CEdit m_dueno;
	CEdit m_banco;
	CEdit m_pais;
	CButton m_is_tarjeta;
	CListBox m_paises;
	CListBox m_duenos;
	CListBox m_bancos;
	CListBox m_cuentas;
	CListBox m_expanded_accounts;
public:
	afx_msg void OnBnClickedBPaisAdd();
	afx_msg void OnBnClickedBBancoAdd();
	afx_msg void OnBnClickedBDuenoAdd();
	afx_msg void OnBnClickedBCuentaAdd();
private:
	CEdit m_ubicacion;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLbnSelchangeLCuentas();
	afx_msg void OnLbnSelchangeLBancos();
private:
	CEdit m_concepto;
//	CEdit m_cuenta_destino;
//	CEdit m_cuenta_origen;
	CEdit m_descripcion_linea;
public:
	afx_msg void OnBnClickedBReadStatementline();
private:
	CEdit m_line_date;
	CEdit m_stmt_date;
	CEdit m_amount_local;
	CEdit m_amount_dolares;
	CEdit m_cuenta_propia;
	CEdit m_otra_cuenta;
	CListBox m_cuentas_propias;
public:
	afx_msg void OnLbnSelchangeLDuenos();
	afx_msg void OnBnClickedBDeselectAccounts();
private:
	CListBox m_transacciones;
	CListBox m_conceptos;
public:
	afx_msg void OnBnClickedBDeselectDuenos();
	afx_msg void OnBnClickedBDeselectBancos();
	afx_msg void OnBnClickedBDeselectPais();
	afx_msg void OnBnClickedBConcepto();
	afx_msg void OnLbnSelchangeLConceptos();
	afx_msg void OnBnClickedBDeselectConceptos();
};


