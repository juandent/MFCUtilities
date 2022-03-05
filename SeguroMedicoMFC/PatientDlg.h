#pragma once

#include "BoxContents.h"

// PatientDlg dialog

class PatientDlg : public CDialog
{
	DECLARE_DYNAMIC(PatientDlg)

	BoxContents<Patient, &Patient::id> m_patientLB;
	
	void Refresh();
	std::optional<Patient> m_patient;
	
public:
	PatientDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PatientDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PatientDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_id_patient;
	CEdit m_first_name;
	CEdit m_last_name;
	CListBox m_list_patients;
public:
	afx_msg void OnBnClickedAplicarPaciente();
	afx_msg void OnBnClickedNewPatient();
	afx_msg void OnBnClickedBorrarPatient();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeLPacientes();
};
