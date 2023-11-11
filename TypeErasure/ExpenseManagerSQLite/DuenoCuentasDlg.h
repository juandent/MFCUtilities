#pragma once
#include "BoxContents.h"
#include "Data_Tier.h"
#include "OneToMany.h"


// DuenoCuentasDlg dialog

class DuenoCuentasDlg : public CDialog
{
	DECLARE_DYNAMIC(DuenoCuentasDlg)

	OneToMany < AccountOwner, &AccountOwner::id_owner, Account, &Account::fkey_account_owner, &Account::id_account> OneToN;
public:
	DuenoCuentasDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DuenoCuentasDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DuenoCuentasDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ownersName_box;
	CListBox m_cuentas_list;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeOwnerName();
	afx_msg void OnLbnSelchangeLAccounts();
};
