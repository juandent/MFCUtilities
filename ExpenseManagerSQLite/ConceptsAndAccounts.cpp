// ConceptsAndAccounts.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "ConceptsAndAccounts.h"

#include "Data_Tier.h"

#include <afxext.h>

#include "ListboxContents.h"





// ConceptsAndAccounts

IMPLEMENT_DYNCREATE(ConceptsAndAccounts, CFormView)

ConceptsAndAccounts::ConceptsAndAccounts()
	: CFormView(IDD_ConceptsAndAccounts),
	m_grid_controller(m_statementLines, 10),
	m_paisLB{ m_paises, [](Pais& pais)
{
	auto display = JD::to_cstring(pais.name);
	return display;
} },
m_duenoLB{ m_duenos,
[](AccountOwner& owner)
		{
			auto display = JD::to_cstring(owner.name);
			return display;
		} },
	m_bancoLB{ m_bancos,
	[](Banco& banco)
		{
			auto display = JD::to_cstring(banco.nombre + " - " + banco.ubicacion);
			return display;
		} },
			m_accountLB{ m_cuentas,
			[](Account& account)
				{
					auto display = JD::to_cstring(account.number + " - " + account.description);
					return display;
				} }
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
	DDX_Control(pDX, IDC_E_UBICACION, m_ubicacion);
}

BEGIN_MESSAGE_MAP(ConceptsAndAccounts, CFormView)

	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_B_LOAD_COMPOUND_DOC, &ConceptsAndAccounts::OnBnClickedBLoadCompoundDoc)
	ON_BN_CLICKED(IDC_B_PAIS_ADD, &ConceptsAndAccounts::OnBnClickedBPaisAdd)
	ON_BN_CLICKED(IDC_B_BANCO_ADD, &ConceptsAndAccounts::OnBnClickedBBancoAdd)
	ON_BN_CLICKED(IDC_B_DUENO_ADD, &ConceptsAndAccounts::OnBnClickedBDuenoAdd)
	ON_BN_CLICKED(IDC_B_CUENTA_ADD, &ConceptsAndAccounts::OnBnClickedBCuentaAdd)
	ON_LBN_SELCHANGE(IDC_L_CUENTAS, &ConceptsAndAccounts::OnLbnSelchangeLCuentas)
	ON_LBN_SELCHANGE(IDC_L_BANCOS, &ConceptsAndAccounts::OnLbnSelchangeLBancos)
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

///	m_grid_controller.OnInitialUpdate();
///	m_banco.SetWindowTextW(L"BANCO");
	
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



void ConceptsAndAccounts::OnBnClickedBPaisAdd()
{
	// TODO: Add your control notification handler code here
	CString rStr;
	m_pais.GetWindowTextW(rStr);

	auto name = JD::from_cstring(rStr);

	auto whereClause = c(&Pais::name) == name.c_str();
	
	std::optional<Pais> pais = m_paisLB.exists(whereClause, &Pais::id_pais, &Pais::name);

	if( ! pais)
	{
		pais = m_paisLB.insert(name);
		m_paisLB.insert_into_listbox(*pais);
	}
}


// banco --> nombre, ubicacion

void ConceptsAndAccounts::OnBnClickedBBancoAdd()
{
	// TODO: Add your control notification handler code here
	CString rBanco, rUbicacion;
	m_banco.GetWindowTextW(rBanco);
	m_ubicacion.GetWindowTextW(rUbicacion);

	auto name = JD::from_cstring(rBanco);
	auto ubicacion = JD::from_cstring(rUbicacion);

	auto whereClause = (c(&Banco::nombre) == name.c_str()) && (c(&Banco::ubicacion) == ubicacion.c_str());

	std::optional<Banco> banco = m_bancoLB.exists(whereClause, &Banco::id_bank, &Banco::nombre);

	if (!banco)
	{
		// need to get current pais
		std::optional<Pais> pais = m_paisLB.current();
		if (!pais)
		{
			banco = m_bancoLB.insert(name, ubicacion);
			m_bancoLB.insert_into_listbox(*banco);
		}
	}


	//m_bancos.AddString(rStr);
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


void ConceptsAndAccounts::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();


	m_paisLB.loadLB();
	m_duenoLB.loadLB();
	m_accountLB.loadLB();
	m_bancoLB.loadLB();
}


void ConceptsAndAccounts::OnLbnSelchangeLCuentas()
{
	// TODO: Add your control notification handler code here
	std::optional<Account> act = m_accountLB.current();
	if( act)
	{
		std::optional<AccountOwner> owner = m_duenoLB.select(act->fkey_account_owner);
		std::optional<Banco> banco = m_bancoLB.select(act->fkey_bank);
	
	}
}


void ConceptsAndAccounts::OnLbnSelchangeLBancos()
{
	// TODO: Add your control notification handler code here
	std::optional<Banco> banco = m_bancoLB.current();
	if( banco )
	{
		std::optional<Pais> pais = m_paisLB.select(banco->fkey_pais);
	}
}


BOOL ConceptsAndAccounts::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::OnChildNotify(message, wParam, lParam, pLResult);
}
