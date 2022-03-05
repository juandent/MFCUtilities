#pragma once
#include <afxwin.h>

#include "BoxContents.h"

// MedicationDlg dialog

class CWnd;

class MedicationDlg : public CDialog
{
	DECLARE_DYNAMIC(MedicationDlg)

	BoxContents<Medication, &Medication::id> m_medicationLB;

	std::optional<Medication> m_medication;
	
	void Refresh();
public:
	MedicationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MedicationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MedicationDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_id_medication;
	CEdit m_name;
	CListBox m_list_medications;
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedNuevo();
	afx_msg void OnBnClickedBorrar();
	afx_msg void OnBnClickedCerrar();
	afx_msg void OnLbnSelchangeLMedications();
};
