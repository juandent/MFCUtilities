#pragma once

#include <JDGridCtrl.h>

import idisplayer;

// MainView form view

class MainView : public CFormView
{
	DECLARE_DYNCREATE(MainView)

protected:
	MainView();           // protected constructor used by dynamic creation
	virtual ~MainView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINVIEW };
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

	CJDGridCtrl m_grid_claim;
	std::unique_ptr<IDisplayer> m_displayer_claims;
	template<typename T>
	void InitializeGridClaims(const T& t);


	CJDGridCtrl m_grid_facturas;
	std::unique_ptr<IDisplayer> m_displayer_responses;
	template <class T>
	void InitializeGridINSResponses(const T& t);

	CJDGridCtrl m_grid_pending;
	std::unique_ptr<IDisplayer> m_displayer_pending_invoices;
	template <class T>
	void InitializeGridPendingInvoices(const T& t);
	CDateTimeCtrl m_fecha_inicial;
	CDateTimeCtrl m_fecha_final;
};


