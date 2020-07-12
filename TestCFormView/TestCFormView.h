
// TestCFormView.h : main header file for the TestCFormView application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTestCFormViewApp:
// See TestCFormView.cpp for the implementation of this class
//

class CTestCFormViewApp : public CWinApp
{
public:
	CTestCFormViewApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTestCFormViewApp theApp;
