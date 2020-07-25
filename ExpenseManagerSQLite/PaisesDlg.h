#pragma once
#include "Data_Tier.h"
#include "boxContents.h"


// PaisesDlg dialog

class PaisesDlg : public CDialog
{
	DECLARE_DYNAMIC(PaisesDlg)
	BoxContents<Pais, &Pais::id_pais> m_paisLB;
public:
	PaisesDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PaisesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PaisesDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CListBox m_list_paises;
public:
	afx_msg void OnBnClickedOk();
private:
	CEdit m_id_pais;
public:
	afx_msg void OnBnClickedBAplicarPaises();
private:
	CEdit m_nombre_pais;
public:
	std::optional<Pais> m_pais{ std::nullopt };
	afx_msg void OnLbnSelchangeLPaises();
	afx_msg void OnBnClickedBBorrar();
};
