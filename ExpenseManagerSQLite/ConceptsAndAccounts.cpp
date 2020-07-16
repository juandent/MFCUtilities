// ConceptsAndAccounts.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "ConceptsAndAccounts.h"

#include <afxext.h>


// ConceptsAndAccounts

IMPLEMENT_DYNCREATE(ConceptsAndAccounts, CFormView)

ConceptsAndAccounts::ConceptsAndAccounts()
	: CFormView(IDD_ConceptsAndAccounts),
	m_grid_controller(m_statementLines, 10)
{

}

ConceptsAndAccounts::~ConceptsAndAccounts()
{
}

void ConceptsAndAccounts::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_statementLines);
	DDX_Control(pDX, IDC_E_ACCOUNT_NUMBER, m_account_number);
	DDX_Control(pDX, IDC_E_ACCOUNT_DESC, m_account_description);
	DDX_Control(pDX, IDC_E_DUENO, m_dueno);
	DDX_Control(pDX, IDC_E_BANCO, m_banco);
	DDX_Control(pDX, IDC_E_PAIS, m_pais);
	DDX_Control(pDX, IDC_C_IS_TARJETA, m_is_tarjeta);
	DDX_Control(pDX, IDC_L_PAISES, m_paises);
	DDX_Control(pDX, IDC_L_DUENOS, m_duenos);
	DDX_Control(pDX, IDC_L_BANCOS, m_bancos);
	DDX_Control(pDX, IDC_L_CUENTAS, m_cuentas);
	DDX_Control(pDX, IDC_L_EXPANDED_ACCOUNTS, m_expanded_accounts);
}

BEGIN_MESSAGE_MAP(ConceptsAndAccounts, CFormView)

	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_B_LOAD_COMPOUND_DOC, &ConceptsAndAccounts::OnBnClickedBLoadCompoundDoc)
	ON_BN_CLICKED(IDC_B_PAIS_ADD, &ConceptsAndAccounts::OnBnClickedBPaisAdd)
	ON_BN_CLICKED(IDC_B_BANCO_ADD, &ConceptsAndAccounts::OnBnClickedBBancoAdd)
	ON_BN_CLICKED(IDC_B_DUENO_ADD, &ConceptsAndAccounts::OnBnClickedBDuenoAdd)
	ON_BN_CLICKED(IDC_B_CUENTA_ADD, &ConceptsAndAccounts::OnBnClickedBCuentaAdd)
END_MESSAGE_MAP()


// ConceptsAndAccounts diagnostics

#ifdef _DEBUG
void ConceptsAndAccounts::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ConceptsAndAccounts::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ConceptsAndAccounts message handlers


int ConceptsAndAccounts::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	/////m_grid_controller.OnInitialUpdate();
	
	return 0;
}


void ConceptsAndAccounts::OnBnClickedBLoadCompoundDoc()
{
	// TODO: Add your control notification handler code here
	auto startPath = fs::current_path();
	auto current = startPath.string();
	auto currPath = JD::to_cstring(current);
	CFileDialog dlg{ true };
	auto ret = dlg.DoModal();
	if (ret != 1)	return;
	auto folderPath = dlg.GetPathName();
	auto selectedFileName = JD::from_cstring(folderPath);
	LoadFile(selectedFileName);
}


void ConceptsAndAccounts::LoadFile(const std::string& fileName)
{
	m_file.load(fileName);
	bool isFirstLine = true;
	for( auto& it = m_file.cbegin(); it != m_file.cend(); ++it)
	{
		if( isFirstLine)
		{
			isFirstLine = false;
			continue;
		}
		auto x = *it;

		Translation t;
		t.statement_date = JD::to_date(x[0]);
		t.origin_account = x[1];
		t.line_date = JD::to_date(x[2]);
		t.concepto = x[3];
		t.amount_local = std::stod(x[4]);
		t.amount_dollars = std::stod(x[5]);
		t.is_enabled = x[6] == "1";
		t.category_name = x[7];
		t.description = x[8];
		if( x.size() > 9)
			t.owner_name = x[9];

		m_grid_controller.lines.push_back(t);
	}
	m_grid_controller.OnInitialUpdate();
}

#if 0
void ConceptsAndAccounts::OnBnClickedInsertAccount()
{
	// TODO: Add your control notification handler code here
	CString rStr;
	m_account.GetWindowTextW(rStr);

	m_listOfAccounts.AddString(rStr);
	
}
#endif

void ConceptsAndAccounts::OnBnClickedBPaisAdd()
{
	// TODO: Add your control notification handler code here
	CString rStr;
	m_pais.GetWindowTextW(rStr);

	m_paises.AddString(rStr);
}


void ConceptsAndAccounts::OnBnClickedBBancoAdd()
{
	// TODO: Add your control notification handler code here
	CString rStr;
	m_banco.GetWindowTextW(rStr);

	m_bancos.AddString(rStr);
}


void ConceptsAndAccounts::OnBnClickedBDuenoAdd()
{
	// TODO: Add your control notification handler code here
	CString rStr;
	m_dueno.GetWindowTextW(rStr);

	m_duenos.AddString(rStr);
}


void ConceptsAndAccounts::OnBnClickedBCuentaAdd()
{
	// TODO: Add your control notification handler code here
	CString numero, descripcion;
	bool isTarjeta;

	m_account_number.GetWindowTextW(numero);
	m_account_description.GetWindowTextW(descripcion);
	isTarjeta = m_is_tarjeta.GetCheck();

	CString cuenta = numero;
	cuenta += " :";
	cuenta += descripcion;
	cuenta += " ";
	cuenta += isTarjeta ? "Tarjeta" : "Cuenta";

	m_cuentas.AddString(numero);
	m_expanded_accounts.AddString(cuenta);
}
