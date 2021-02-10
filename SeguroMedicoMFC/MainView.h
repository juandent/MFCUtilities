#pragma once

#include "..\ExpenseManagerSQLite/IDisplayer.h"
#include "GridDisplayer.h"
#include "Data.h"


// MainView form view

class MainView : public CFormView
{
	DECLARE_DYNCREATE(MainView)

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
	void InitializeGrid();
private:
	CJDGridCtrl m_grid_1;
	std::unique_ptr<GridDisplayer<Patient>> m_displayer_patients;
};


