
// TesterForMFCUtilitiesDLL.h : main header file for the TesterForMFCUtilitiesDLL application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTesterForMFCUtilitiesDLLApp:
// See TesterForMFCUtilitiesDLL.cpp for the implementation of this class
//

namespace Controller
{
	class CTesterForMFCUtilitiesDLLApp : public CWinAppEx
	{
	public:
		CTesterForMFCUtilitiesDLLApp() noexcept;


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
		afx_msg void OnFileExercise();
		afx_msg void OnFileLoadCompoundDoc();
		afx_msg void OnFileOpenownersdialog();
		afx_msg void OnLoadCategories();
		afx_msg void OnDatabaseEmptydb();
		afx_msg void OnDatabaseStatementlines();
		afx_msg void OnDatabasePersons();
	};

	extern CTesterForMFCUtilitiesDLLApp theApp;

}

using Controller::theApp;
