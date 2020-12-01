#pragma once

#include "ExpenseManagerSQLite/Data_Tier.h"
#include "TesterForMFCUtilitiesDLL/JoinedGridDisplayer.h"


// JoinedGridDisplayerView form view
template<auto N> constexpr auto val = N;

class JoinedGridDisplayerView : public CFormView
{
	DECLARE_DYNCREATE(JoinedGridDisplayerView)
	
	struct WhereParameters
	{
		JoinedGridDisplayerView*	view;
		std::optional<std::string>	conceptoPattern {"%"};
		std::optional<int>			fkey_statement;
		void executeWhere();
	} whereParameters{ this };
protected:
	JoinedGridDisplayerView();           // protected constructor used by dynamic creation
	virtual ~JoinedGridDisplayerView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JoinedGridDisplayerView };
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
	CJDGridCtrl m_grid;
	std::unique_ptr<IDisplayer> m_displayer;
	std::unique_ptr<IDisplayer> m_combo_displayer;
public:
	virtual void OnInitialUpdate();
private:
	CComboBox m_statement_dates;
	template<typename T>
	void InitializeGrid(const T& t);
public:
	afx_msg void OnCbnSelchangeCStatementdates();
private:
	CEdit m_conceptoSearch;
public:
	afx_msg void OnEnKillfocusEConcepto();
	afx_msg void OnBnClickedBFilter();
private:
	CEdit m_countMainGrid;
};


