#pragma once

#include "BoxContents.h"

// DoctorDlg dialog

class DoctorDlg : public CDialog
{
	DECLARE_DYNAMIC(DoctorDlg)

	BoxContents<Doctor, &Doctor::id>  m_doctoresLB;
	BoxContents<Specialty, &Specialty::id, CComboBox > m_specialtiesCB;
	std::optional<Doctor> m_doctor;
	
	void Refresh();

public:
	DoctorDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DoctorDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DoctorDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_id_doctor;
	CEdit m_first_name;
	CEdit m_last_name;
	CComboBox m_cblist_Specialties;
	CListBox m_list_doctores;
public:
	afx_msg void OnLbnSelchangeLDoctores();
	afx_msg void OnCbnSelchangeCbSpecialidades();
	afx_msg void OnBnClickedAplicarDoctor();
	afx_msg void OnBnClickedNewDoctor();
	afx_msg void OnBnClickedBorrarDoctor();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedEditSpecialties();
};
