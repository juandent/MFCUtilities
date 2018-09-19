
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
	};

	extern CTesterForMFCUtilitiesDLLApp theApp;

}

using Controller::theApp;
