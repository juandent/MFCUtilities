// BarChartCtrl.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "BarChartCtrl.h"

#include <afxvisualmanager.h>

#include "BarChartCtrlDlg.h"
#include <afxshellmanager.h>
#include <afxvisualmanagerwindows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarChartCtrlApp

BEGIN_MESSAGE_MAP(CBarChartCtrlApp, CWinApp)
	//{{AFX_MSG_MAP(CBarChartCtrlApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarChartCtrlApp construction

CBarChartCtrlApp::CBarChartCtrlApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBarChartCtrlApp object

CBarChartCtrlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBarChartCtrlApp initialization

BOOL CBarChartCtrlApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
// any shell tree view or shell list view controls.
	CShellManager* pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
#if 0
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	CBarChartCtrlDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
