
// ExpenseManagerSQLite.h : main header file for the ExpenseManagerSQLite application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CExpenseManagerSQLiteApp:
// See ExpenseManagerSQLite.cpp for the implementation of this class
//

class CExpenseManagerSQLiteApp : public CWinAppEx
{
public:
	CExpenseManagerSQLiteApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAssociateConceptsAccounts();
	afx_msg void OnFileCuentadialog();
	afx_msg void OnFileTransacciondialog();
	afx_msg void OnFileBancodialog();
	afx_msg void OnFilePaisdialog();
	afx_msg void OnFileOwner();
	afx_msg void OnFileConceptosdialog();
	afx_msg void OnFileStatementdialog();
	afx_msg void OnFileCategorydialog();
};

extern CExpenseManagerSQLiteApp theApp;
