#pragma once


#include "Data_Tier.h"
#include "BoxContents.h"

// ConceptosDlg dialog

class ConceptosDlg : public CDialog
{
	DECLARE_DYNAMIC(ConceptosDlg)

	BoxContents<Concepto, &Concepto::id_concepto> m_conceptosLB;
	BoxContents<Account, &Account::id_account, CComboBox> m_accountsCB;

	std::string m_discriminator;	// name
	std::optional<Concepto> m_concepto;
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
	void setDiscriminator(std::string& name)
	{
		m_discriminator = name;
	}

	std::optional<Concepto> getCompleteObject() const
	{
		return m_concepto;
	}

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBBorrar();
private:
	CComboBox m_listAccounts;
	CListBox m_listConceptos;
public:
	afx_msg void OnBnClickedBAddAccount();
	afx_msg void OnBnClickedBUpdateConcepto();
private:
	CEdit m_nombre;
public:
	afx_msg void OnLbnSelchangeLConceptos();
	afx_msg void OnBnClickedBAplicarConcepto();
private:
	CEdit m_id;
};
