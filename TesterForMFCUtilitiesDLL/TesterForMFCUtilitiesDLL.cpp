
// TesterForMFCUtilitiesDLL.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TesterForMFCUtilitiesDLL.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "TesterForMFCUtilitiesDLLDoc.h"
#include "TesterForMFCUtilitiesDLLView.h"
#include "OwnersView.h"
#include "OwnerDoc.h"
#include "OwnersDialog.h"


#include "../Model/Model.DataTier.h"


#include "CSVFile.h"
#include <filesystem>
#include <string>

#include "ORM/ORM.Repository.h"

#include "Utilities.h"
#include <any>
#include "Controllers/Controller.LoadCompoundDocIntoDB.h"
#include "DocViewTemplates.h"
#include "Repositories/CategoryRepository.h"


using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace View
{
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
}





namespace Controller
{


	// CTesterForMFCUtilitiesDLLApp

	BEGIN_MESSAGE_MAP(CTesterForMFCUtilitiesDLLApp, CWinAppEx)
		ON_COMMAND(ID_APP_ABOUT, &CTesterForMFCUtilitiesDLLApp::OnAppAbout)
		// Standard file based document commands
		ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
		ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
		// Standard print setup command
		ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
		ON_COMMAND(ID_FILE_EXERCISE, &CTesterForMFCUtilitiesDLLApp::OnFileExercise)
		ON_COMMAND(ID_FILE_LOADCOMPOUNDDOC, &CTesterForMFCUtilitiesDLLApp::OnFileLoadCompoundDoc)
		ON_COMMAND(ID_FILE_OPENOWNERSDIALOG, &CTesterForMFCUtilitiesDLLApp::OnFileOpenownersdialog)
		ON_COMMAND(ID_LOAD_CATEGORIES, &CTesterForMFCUtilitiesDLLApp::OnLoadCategories)
		ON_COMMAND(ID_DATABASE_EMPTYDB, &CTesterForMFCUtilitiesDLLApp::OnDatabaseEmptydb)
		ON_COMMAND(ID_DATABASE_STATEMENTLINES, &CTesterForMFCUtilitiesDLLApp::OnDatabaseStatementlines)
		ON_COMMAND(ID_DATABASE_PERSONS, &CTesterForMFCUtilitiesDLLApp::OnDatabasePersons)
	END_MESSAGE_MAP()


	// CTesterForMFCUtilitiesDLLApp construction

	CTesterForMFCUtilitiesDLLApp::CTesterForMFCUtilitiesDLLApp() noexcept
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
		SetAppID(_T("TesterForMFCUtilitiesDLL.AppID.NoVersion"));

		// TODO: add construction code here,
		// Place all significant initialization in InitInstance
	}

	// The one and only CTesterForMFCUtilitiesDLLA

	CTesterForMFCUtilitiesDLLApp theApp;


	// CTesterForMFCUtilitiesDLLApp initialization

	BOOL CTesterForMFCUtilitiesDLLApp::InitInstance()
	{
		ORM::Storage::initialize();


#if 1
		using namespace sqlite_orm;


		// auto collect = ORM::Storage::getStorage().get_all<Model::Category>(order_by(&Model::Category::m_name_id), order_by(&Model::Category::m_real_expense_or_income));
		// for( auto& el : collect)
		// {
		// 	auto id = el.m_name_id;
		// 	auto cc = el.m_real_expense_or_income;
		// 	int i = 0;
		// }
		
		auto collection = getCategoriesSortedBy(&Model::Category::m_name_id, &Model::Category::m_real_expense_or_income);
		for (auto& el : collection)
		{
			auto id = el.m_name_id;
			auto cc = el.m_real_expense_or_income;
			int i = 0;
		}


		
		CategoryRepository cat_repo{};
		auto res = cat_repo.allCategoriesSortedBy( order_by(&Model::Category::m_name_id) );
#endif

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

		// Register the application's document templates.  Document templates
		//  serve as the connection between documents, frame windows and views
		CMultiDocTemplate* pDocTemplate;
		pDocTemplate = new CMultiDocTemplate(IDR_TesterForMFCUtilitiesDLLTYPE,
			RUNTIME_CLASS(CTesterForMFCUtilitiesDLLDoc),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			RUNTIME_CLASS(CTesterForMFCUtilitiesDLLView));
		if (!pDocTemplate)
			return FALSE;
		AddDocTemplate(pDocTemplate);
		{
			CMultiDocTemplate* pDocTemplate;
			pDocTemplate = new CMultiDocTemplate(IDR_TesterForMFCUtilitiesDLLTYPE2,
				RUNTIME_CLASS(OwnerDoc),
				RUNTIME_CLASS(CChildFrame), // custom MDI child frame
				RUNTIME_CLASS(OwnersView));
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

#if 1
		void doIT();
		doIT();
		void doITJoined();
		doITJoined();
#endif

		// Parse command line for standard shell commands, DDE, file open
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		// Dispatch commands specified on the command line.  Will return FALSE if
		// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
		// The main window has been initialized, so show and update it
		pMainFrame->ShowWindow(m_nCmdShow);
		pMainFrame->UpdateWindow();
		return TRUE;
	}

	int CTesterForMFCUtilitiesDLLApp::ExitInstance()
	{
		//TODO: handle additional resources you may have added
		AfxOleTerm(FALSE);

		return CWinAppEx::ExitInstance();
	}

	// CTesterForMFCUtilitiesDLLApp message handlers


// App command to run the dialog
	void CTesterForMFCUtilitiesDLLApp::OnAppAbout()
	{
		View::CAboutDlg aboutDlg;
		aboutDlg.DoModal();
	}

	// CTesterForMFCUtilitiesDLLApp customization load/save methods

	void CTesterForMFCUtilitiesDLLApp::PreLoadState()
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

	void CTesterForMFCUtilitiesDLLApp::LoadCustomState()
	{
	}

	void CTesterForMFCUtilitiesDLLApp::SaveCustomState()
	{
	}

	// CTesterForMFCUtilitiesDLLApp message handlers


	void CTesterForMFCUtilitiesDLLApp::OnFileExercise()
	{
		// TODO: Add your command handler code here
		using namespace sqlite_orm;
		using namespace Model;

		std::any b = 4.3;
		if (b.type() == typeid(double))
		{
			double d = std::any_cast<double>(b);
			d++;
		}

		std::vector<Model::Category> categories;
		std::vector<Model::StatementLine> statementlines;

		CategoryRepository repo;
		auto cats = repo.allCategoriesSortedBy(order_by(&Category::m_name_id));
		auto cats2 = repo.getCategoriesSortedBy(&Category::m_name_id);
		getCategoriesSortedBy(&Category::m_name_id);

		//auto collection = ORM::Storage::getStorage().get_all<Model::Category>(order_by(&Category::m_name_id));


#if 0
		namespace fs = std::filesystem;

		auto p = fs::temp_directory_path();
		p.append("Transacciones del mes.csv"s);

		CSVFile fileReader;
		fileReader.load(p.string());
		auto res = fileReader.getText(0, 0);
		auto res2 = fileReader.getText(1, 0);

		int i = 0;
#endif
	}


	void CTesterForMFCUtilitiesDLLApp::OnFileLoadCompoundDoc()
	{
		// TODO: Add your command handler code here
		CString title = L"Escoger archivos CSV para cargar a la base de datos";

		constexpr int MAX_FILE_COUNT = 99;
		constexpr int FILE_LIST_BUFFER_SIZE = (MAX_FILE_COUNT * (MAX_PATH + 1)) + 1;

		JD::Buffer<wchar_t> buffer{ FILE_LIST_BUFFER_SIZE };


		CFileDialog fileChooser{ true, L"*.csv", NULL, 0 /*OFN_ALLOWMULTISELECT*/, L"CSV Files (*.csv)|*.csv||" };
		fileChooser.m_ofn.lpstrTitle = title.operator LPCWSTR();
		fileChooser.m_ofn.lpstrFile = buffer; // file_names_buffer.data();
		fileChooser.m_ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

		auto	ret = fileChooser.DoModal();

		if (ret != 1)
			return;

		std::vector<wstring> chosen_files;
		CString cs_file_name = fileChooser.GetPathName();
		auto file_name = JD::to_string(cs_file_name.GetString());

		Controller::LoadCompoundDocIntoDB loader(file_name);
		loader.DoLoadIntoDB();

		//auto pos = fileChooser.GetStartPosition();
		//while(pos)
		//{
		//	CString file_name = fileChooser.GetNextPathName(pos);
		//	auto name = file_name.GetString();
		//	chosen_files.push_back(name);
		//}
	}


	void CTesterForMFCUtilitiesDLLApp::OnFileOpenownersdialog()
	{
		// TODO: Add your command handler code here
		auto dialog = OwnersDialog{};
		dialog.DoModal();
	}


	void CTesterForMFCUtilitiesDLLApp::OnLoadCategories()
	{
		// TODO: Add your command handler code here
	}


	void CTesterForMFCUtilitiesDLLApp::OnDatabaseEmptydb()
	{
		// TODO: Add your command handler code here
		ORM::Storage::empty_database();
	}



	void CTesterForMFCUtilitiesDLLApp::OnDatabaseStatementlines()
	{
		// TODO: Add your command handler code here
		DocViewTemplates templates{};
		if (!templates.OpenNullFile(L"StatementLines"))
			return;
	}


	void CTesterForMFCUtilitiesDLLApp::OnDatabasePersons()
	{
		// TODO: Add your command handler code here

	}
}
