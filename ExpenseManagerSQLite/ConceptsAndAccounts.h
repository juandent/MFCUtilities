#pragma once
#include "CSVFile.h"
#include "StatementLineGridController.h"


// ConceptsAndAccounts form view

class ConceptsAndAccounts : public CFormView
{
	DECLARE_DYNCREATE(ConceptsAndAccounts)

protected:
	ConceptsAndAccounts();           // protected constructor used by dynamic creation
	virtual ~ConceptsAndAccounts();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ConceptsAndAccounts };
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
	CJDGridCtrl m_statementLines;
	StatementLineGridController m_grid_controller;
	CSVFile m_file;
	void LoadFile(const std::string& fileName);
public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedBLoadCompoundDoc();
private:
	CListBox m_listOfAccounts;
};


