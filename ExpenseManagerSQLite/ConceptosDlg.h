#pragma once


#include "Data_Tier.h"
#include "BoxContents.h"

// ConceptosDlg dialog

class ConceptosDlg : public CDialog
{
	DECLARE_DYNAMIC(ConceptosDlg)

	BoxContents<Concepto, &Concepto::id_concepto> m_conceptosLB;
	BoxContents<Account, &Account::id_account, CComboBox> m_accountsCB;

public:
	ConceptosDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ConceptosDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ConceptosDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBBorrar();
private:
	CComboBox m_listAccounts;
	CListBox m_listConceptos;
};
