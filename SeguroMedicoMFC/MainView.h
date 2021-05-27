#pragma once

#include "..\ExpenseManagerSQLite/IDisplayer.h"
#include "..\TesterForMFCUtilitiesDLL/GridDisplayer.h"
#include "Data.h"
#include "..\TesterForMFCUtilitiesDLL/JoinedGridDisplayer.h"
#include "BoxContents.h"

// MainView form view

class MainView : public CFormView
{
	DECLARE_DYNCREATE(MainView)

	BoxContents<Doctor, &Doctor::id, CComboBox> m_doctoresCB;
	BoxContents<Patient, &Patient::id, CComboBox> m_pacientesCB;
	BoxContents<Claim, &Claim::id, CComboBox> m_claimsCB;

	enum { ID_REEMBOLSO_COLUMN=1};
	
protected:
	MainView();           // protected constructor used by dynamic creation
	virtual ~MainView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainView };
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
public:
	virtual void OnInitialUpdate();
	void Refresh();
	template <class T> void InitializeGridClaims(const T &t);
	template <class T> void InitializeGridINSResponses(const T& t);
	auto GetWhereStatement();
private:
	CJDGridCtrl m_grid_1;
	std::unique_ptr<GridDisplayer<Patient>> m_displayer_patients;
	std::unique_ptr<IDisplayer> m_displayer_claims;
	std::unique_ptr<IDisplayer> m_displayer_responses;
	std::unique_ptr<IDisplayer> m_displayer_claims_combobox;
	CDateTimeCtrl m_start_date;
	CDateTimeCtrl m_end_date;
public:
	afx_msg void OnBnClickedBFilter();
private:
	CComboBox m_lista_doctores;
	CComboBox m_lista_pacientes;
	CButton m_filter_by_dates;
	CEdit m_claim_amount;
public:
	afx_msg void OnBnClickedBFilterInsresponses();
private:
	CJDGridCtrl m_grid_2;
	CComboBox m_list_claim_ids;
public:
	afx_msg void OnBnClickedBClearFilters();
	afx_msg void OnBnClickedBRefresh();
	afx_msg void OnFilePrint();
	void OnGrid2StartSelChange(NMHDR* pNotifyStruct, LRESULT*);
	void OnGrid1StartSelChange(NMHDR* pNotifyStruct, LRESULT*);
private:
	CEdit m_num_Factura;
public:
	afx_msg void OnBnClickedBSelectReembolso();
	afx_msg void OnBnClickedBCrearReembolso();
private:
	CEdit m_invoice_description;
};


