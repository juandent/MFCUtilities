#pragma once

// ClaimView form view

class ClaimDoc;

class ClaimView : public CFormView
{
	DECLARE_DYNCREATE(ClaimView)

	BoxContents<Patient, &Patient::id, CComboBox> m_patientsCB;
	BoxContents<Doctor, &Doctor::id, CComboBox> m_doctorsCB;
	BoxContents<Medication, &Medication::id, CComboBox> m_medicationsCB;
protected:
	ClaimView();           // protected constructor used by dynamic creation
	virtual ~ClaimView();

public:
	ClaimDoc* GetDocument();
	
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ClaimView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
//	CEdit m_id_claim;
	CComboBox m_patients;
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnBnClickedApply();
private:
	CJDGridCtrl m_grid1;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnCbnSelchangeCPatients();
private:
	CDateTimeCtrl m_start_date;
public:
	afx_msg void OnDtnDatetimechangeStartDate(NMHDR* pNMHDR, LRESULT* pResult);
private:
	CComboBox m_doctors;
	CComboBox m_medicamentos;
	CString m_claim_id;
	CDateTimeCtrl m_submitted_date;
	CString m_asprose_claim_number;
	CString m_asprose_case_number;
	CString m_amount_presented;
	CButton m_acknowledgement;
	CString m_other_system_id;
	CString m_ins_claim_number;
	CListBox m_claims;
	CString m_total_claim_amount;
	CString m_comments;
};


