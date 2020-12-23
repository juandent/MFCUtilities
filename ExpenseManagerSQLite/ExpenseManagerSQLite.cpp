
// ExpenseManagerSQLite.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "DuenoCuentasDlg.h"


import Util;

#include <catch2/catch.hpp>
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ExpenseManagerSQLite.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ExpenseManagerSQLiteDoc.h"
#include "ExpenseManagerSQLiteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "AssociateConceptAccount.h"
#include "BancoDlg.h"
#include "CategoryDlg.h"
#include "ConceptosDlg.h"
#include "ConceptsAndAccounts.h"
#include "CuentaDlg.h"
#include "Data_Tier.h"
#include "DuenosDlg.h"
#include "PaisesDlg.h"
#include "StatementDlg.h"
#include "TransaccionDlg.h"

#include "ExpenseManagerSQLite/JoinedGridDisplayerDoc.h"
#include "ExpenseManagerSQLite/JoinedGridDisplayerView.h"

// CExpenseManagerSQLiteApp

BEGIN_MESSAGE_MAP(CExpenseManagerSQLiteApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CExpenseManagerSQLiteApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_Menu, &CExpenseManagerSQLiteApp::OnAssociateConceptsAccounts)
	ON_COMMAND(ID_FILE_CUENTADIALOG, &CExpenseManagerSQLiteApp::OnFileCuentadialog)
	ON_COMMAND(ID_FILE_TRANSACCIONDIALOG, &CExpenseManagerSQLiteApp::OnFileTransacciondialog)
	ON_COMMAND(ID_FILE_BANCODIALOG, &CExpenseManagerSQLiteApp::OnFileBancodialog)
	ON_COMMAND(ID_FILE_PAISDIALOG, &CExpenseManagerSQLiteApp::OnFilePaisdialog)
	ON_COMMAND(ID_FILE_OWNER, &CExpenseManagerSQLiteApp::OnFileOwner)
	ON_COMMAND(ID_FILE_CONCEPTOSDIALOG, &CExpenseManagerSQLiteApp::OnFileConceptosdialog)
	ON_COMMAND(ID_FILE_STATEMENTDIALOG, &CExpenseManagerSQLiteApp::OnFileStatementdialog)
	ON_COMMAND(ID_FILE_CATEGORYDIALOG, &CExpenseManagerSQLiteApp::OnFileCategorydialog)
	// ON_COMMAND(ID_FILE_CUENTASDEDUE32782, &CExpenseManagerSQLiteApp::OnFileCuentasdedueno)
	ON_COMMAND(ID_FILE_CUENTASDEDUENO, &CExpenseManagerSQLiteApp::OnFileCuentasdeDuenoDlg)
END_MESSAGE_MAP()


// CExpenseManagerSQLiteApp construction

CExpenseManagerSQLiteApp::CExpenseManagerSQLiteApp() noexcept
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ExpenseManagerSQLite.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CExpenseManagerSQLiteApp object

CExpenseManagerSQLiteApp theApp;


// CExpenseManagerSQLiteApp initialization
// void doIT();

BOOL CExpenseManagerSQLiteApp::InitInstance()
{
	Storage::initialize();
	// doIT();
	
	//Storage::fill_db_with_test_data();
	
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);


	{
		// Register the application's document templates.  Document templates
		//  serve as the connection between documents, frame windows and views
		CMultiDocTemplate* pDocTemplate;
		pDocTemplate = new CMultiDocTemplate(IDR_ExpenseManagerSQLiteTYPE,
			RUNTIME_CLASS(CExpenseManagerSQLiteDoc),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			RUNTIME_CLASS(CExpenseManagerSQLiteView));
		if (!pDocTemplate)
			return FALSE;
		AddDocTemplate(pDocTemplate);
	}
	{
		CMultiDocTemplate* pDocTemplate;
		pDocTemplate = new CMultiDocTemplate(IDR_ExpenseManagerSQLiteTYPE2,
			RUNTIME_CLASS(AssociateConceptAccount),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			RUNTIME_CLASS(ConceptsAndAccounts));
		if (!pDocTemplate)
			return FALSE;
		AddDocTemplate(pDocTemplate);
	}
	{
		CMultiDocTemplate* pDocTemplate;
		pDocTemplate = new CMultiDocTemplate(IDR_ExpenseManagerSQLiteTYPE3,
			RUNTIME_CLASS(JoinedGridDisplayerDoc),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			RUNTIME_CLASS(JoinedGridDisplayerView));
		if (!pDocTemplate)
			return FALSE;
		AddDocTemplate(pDocTemplate);
	}


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CExpenseManagerSQLiteApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CExpenseManagerSQLiteApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CExpenseManagerSQLiteApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CExpenseManagerSQLiteApp customization load/save methods

void CExpenseManagerSQLiteApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CExpenseManagerSQLiteApp::LoadCustomState()
{
}

void CExpenseManagerSQLiteApp::SaveCustomState()
{
}

// CExpenseManagerSQLiteApp message handlers





void CExpenseManagerSQLiteApp::OnAssociateConceptsAccounts()
{
	// TODO: Add your command handler code here
	auto pos = GetFirstDocTemplatePosition();

	CDocTemplate* docTempl = GetNextDocTemplate(pos);
	CString str;
	docTempl->GetDocString(str, CDocTemplate::docName);


	while (str != L"ConceptsAndAccounts" && pos != NULL)
	{
		docTempl = GetNextDocTemplate(pos);
		docTempl->GetDocString(str, CDocTemplate::docName);
	}
	if (docTempl != NULL)
		docTempl->OpenDocumentFile(NULL);
}

void CExpenseManagerSQLiteApp::OnFileCuentadialog()
{
	// TODO: Add your command handler code here
	CuentaDlg dlg;
	dlg.DoModal();
}


void CExpenseManagerSQLiteApp::OnFileTransacciondialog()
{
	// TODO: Add your command handler code here
	TransaccionDlg dlg;
	dlg.DoModal();
}


void CExpenseManagerSQLiteApp::OnFileBancodialog()
{
	// TODO: Add your command handler code here
	BancoDlg dlg;
	dlg.DoModal();
}


void CExpenseManagerSQLiteApp::OnFilePaisdialog()
{
	// TODO: Add your command handler code here
	PaisesDlg dlg;
	dlg.DoModal();
	auto pais = dlg.m_pais;
	
}


void CExpenseManagerSQLiteApp::OnFileOwner()
{
	// TODO: Add your command handler code here
	DuenosDlg dlg;
	dlg.DoModal();
}


void CExpenseManagerSQLiteApp::OnFileConceptosdialog()
{
	// TODO: Add your command handler code here
	ConceptosDlg dlg;
	dlg.DoModal();
}


void CExpenseManagerSQLiteApp::OnFileStatementdialog()
{
	// TODO: Add your command handler code here
	StatementDlg dlg;
	dlg.DoModal();
}


void CExpenseManagerSQLiteApp::OnFileCategorydialog()
{
	// TODO: Add your command handler code here
	// auto dlg = new CategoryDlg;
	// auto ret = dlg->Create(IDD_CategoryDlg);
	// dlg->ShowWindow(SW_SHOW);
	CategoryDlg dlg;
	dlg.DoModal();
}


void CExpenseManagerSQLiteApp::OnFileCuentasdeDuenoDlg()
{
	// TODO: Add your command handler code here
	DuenoCuentasDlg dlg;
	dlg.DoModal();
}
