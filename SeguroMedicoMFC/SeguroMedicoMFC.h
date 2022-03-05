
// SeguroMedicoMFC.h : main header file for the SeguroMedicoMFC application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSeguroMedicoMFCApp:
// See SeguroMedicoMFC.cpp for the implementation of this class
//

class CSeguroMedicoMFCApp : public CWinAppEx
{
public:
	CSeguroMedicoMFCApp() noexcept;


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
	afx_msg void OnFileSpecialtydlg();
	afx_msg void OnFileDoctordlg();
	afx_msg void OnFilePatientdlg();
	afx_msg void OnFileMedicationdlg();
	afx_msg void OnFileClaimdlg();
	afx_msg void OnFileInvoicedlg();
	afx_msg void OnFileInsresponsedlg();
	afx_msg void OnFileInsresponselinedlg();
};

extern CSeguroMedicoMFCApp theApp;
