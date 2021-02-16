#pragma once

#include "BoxContents.h"
#include "RadioButtonGroup.h"
#include "IDisplayer.h"

// ClaimDlg dialog

class ClaimDlg : public CDialog
{
	DECLARE_DYNAMIC(ClaimDlg)

	BoxContents<Claim, &Claim::id> m_claimLB;
	BoxContents<Doctor, &Doctor::id, CComboBox> m_doctorCB;
	BoxContents<Patient, &Patient::id, CComboBox> m_patientCB;
	BoxContents<Medication, &Medication::id, CComboBox> m_medicationCB;

	std::optional<Claim> m_claim;
	RadioButtonGroup m_acknowledgement;

	struct WhereParameters
	{
		ClaimDlg* dlg;
		// std::optional<std::string>	conceptoPattern{ "%" };
		std::optional<int>			claim_id;
		// std::optional<std::string>	categoriaPattern;
		void executeWhere();
	} whereParameters{ this };


	void Refresh();
	template <class T>
	void InitializeGrid(const T& t);

	std::unique_ptr<IDisplayer> m_displayer;

public:
	ClaimDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ClaimDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ClaimDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_id_reclamo;
	CComboBox m_patient_list;
	CComboBox m_doctor_list;
	CComboBox m_medicamento_list;
	
	CDateTimeCtrl m_start_date;
	CDateTimeCtrl m_date_submitted;
	CEdit m_asprose_claim_number;
	CEdit m_asprose_case_number;
	CEdit m_asprose_amount_recognized;
	CButton m_acknowledge_full;
	CButton m_acknowledge_partial;
	CButton m_acknowledge_none;
	CEdit m_ins_claim_number;
	CEdit m_total_claim_amount;
	CEdit m_comment;
	CJDGridCtrl m_grid_claims;
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedNuevo();
	afx_msg void OnBnClickedBorrar();
	afx_msg void OnBnClickedCerrar();
private:
	CListBox m_claim_list;
public:
	afx_msg void OnLbnSelchangeLClaimList();
	afx_msg void OnBnClickedBFilter();
};
