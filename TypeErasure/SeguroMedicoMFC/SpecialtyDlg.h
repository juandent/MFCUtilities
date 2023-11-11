#pragma once

#include "boxContents.h"
#include "Data.h"

// SpecialtyDlg dialog

class SpecialtyDlg : public CDialog
{
	DECLARE_DYNAMIC(SpecialtyDlg)

	BoxContents<Specialty, &Specialty::id> m_specialtyLB;

	std::optional<Specialty> m_specialty;

	void Refresh();
	
public:
	SpecialtyDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SpecialtyDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SpecialtyDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_id_specialty;
	CEdit m_nombre_specialty;
	CListBox m_specialty_list;
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedBorrar();
	afx_msg void OnBnClickedCerrar();
	afx_msg void OnLbnSelchangeLSpecialties();
	afx_msg void OnBnClickedNuevo();
};
