#pragma once
#include <afxwin.h>

#include "BoxContents.h"


// CuentaDlg dialog

class CuentaDlg : public CDialog
{
	DECLARE_DYNAMIC(CuentaDlg)
	BoxContents<Banco, &Banco::id_bank, CComboBox> m_bancosCB;
	BoxContents<AccountOwner, &AccountOwner::id_owner, CComboBox>	m_ownerCB;
	BoxContents<Account, &Account::id_account> m_cuentasLB;

	std::string m_numero_val;	// discriminator
	std::optional<Account> getCurrent() const;
	Operation whatOperation() const;
public:
	CuentaDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CuentaDlg();

	void set_discriminator(const std::string& numero)
	{
		m_numero_val = numero;
	}
	std::optional<Account> m_account;
	auto getCompleteObject()
	{
		return m_account;
	}
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CuentaDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_id_cuenta;
	CEdit m_descripcion;
	CEdit m_numero;
	CComboBox m_dueno_combo;
public:
	afx_msg void OnBnClickedBAddDuenos();
private:
	CButton m_tarjeta;
	CListBox m_list_cuentas;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBAddBancos();
private:
	CComboBox m_banco_combo;
public:
	afx_msg void OnBnClickedBAplicarCuenta();
private:
	CButton m_cuenta_bancaria;
public:
	afx_msg void OnBnClickedBBorrarCuenta();
	afx_msg void OnBnClickedBUpdateAccount();
	afx_msg void OnLbnSelchangeLCuentas();
};
