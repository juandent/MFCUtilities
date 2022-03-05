// ConceptosDlg.cpp : implementation file
//

#include "stdafx.h"

import Util;

#include "ExpenseManagerSQLite.h"
#include "ConceptosDlg.h"
#include "afxdialogex.h"
#include "CuentaDlg.h"
#include "RecordLinks.h"


// ConceptosDlg dialog

IMPLEMENT_DYNAMIC(ConceptosDlg, CDialog)

ConceptosDlg::ConceptosDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ConceptosDlg, pParent),
	m_conceptosLB{ m_listConceptos, [](Concepto& concepto)
	{
		return Util::to_cstring(concepto.id_concepto) + L" - " + Util::to_cstring(concepto.name) + L" - " + Util::to_cstring(concepto.fkey_account);
	}},
	m_accountsCB{ m_listAccounts, [](Account& account)
	{
		return Util::to_cstring(account.id_account) + L" - " + Util::to_cstring(account.number);
	}}
{

}

ConceptosDlg::~ConceptosDlg()
{
}

void ConceptosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_C_ACCOUNT, m_listAccounts);
	DDX_Control(pDX, IDC_L_CONCEPTOS, m_listConceptos);
	DDX_Control(pDX, IDC_E_NOMBRE_CONCEPTO, m_nombre);
	DDX_Control(pDX, IDC_E_ID_CONCEPTO, m_id);
}


BEGIN_MESSAGE_MAP(ConceptosDlg, CDialog)
	ON_BN_CLICKED(ID_B_BORRAR, &ConceptosDlg::OnBnClickedBBorrar)
	ON_BN_CLICKED(IDC_B_ADD_ACCOUNT, &ConceptosDlg::OnBnClickedBAddAccount)
	ON_BN_CLICKED(IDC_B_UPDATE_CONCEPTO, &ConceptosDlg::OnBnClickedBUpdateConcepto)
	ON_LBN_SELCHANGE(IDC_L_CONCEPTOS, &ConceptosDlg::OnLbnSelchangeLConceptos)
	ON_BN_CLICKED(IDC_B_APLICAR_CONCEPTO, &ConceptosDlg::OnBnClickedBAplicarConcepto)
	ON_BN_CLICKED(IDC_C_DESCONOCIDA, &ConceptosDlg::OnBnClickedCDesconocida)
END_MESSAGE_MAP()


// ConceptosDlg message handlers


BOOL ConceptosDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_conceptosLB.loadLBOrderBy(&Concepto::name);
	// m_accountsCB.loadLB();
	// m_listAccounts.GetExStyle()
	m_accountsCB.loadLBOrderBy(&Account::number);
	
	m_nombre.SetWindowTextW(Util::to_cstring(m_discriminator));

	FindNombreInAccountCombo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void ConceptosDlg::FindNombreInAccountCombo()
{
	using namespace std;
#if 0	
	std::string data = "<person>\n"
		" <first>Nico</first>\n"
		" <last>Josuttis</last>\n"
		"</person>\n";

	regex reg("<(.*)>(.*)</(\\1)>");
	sregex_token_iterator pos{ data.cbegin(), data.cend(), reg, {0,2} };
	sregex_token_iterator end;

	string ss{};
	for( ; pos != end; ++pos)
	{
		auto s = pos->str();
		ss += s;
	}

	string names = m_discriminator; // "juan, roberto,leslie;hulse";
	regex sep{ "[ \t\n]*[,;.][ \t\n]*" };
	sregex_token_iterator p{ names.cbegin(), names.cend(), sep, -1 };
	sregex_token_iterator e;

	string pp;
	for( ; p!=e; ++p)
	{
		string _p = *p;
		pp += _p;
	}


	string sdata = "RRERRERR TREEX 3777-11**-****-3030";
	
	
	// TEF A: 9877777777	=> 9877777777
	// TFT DE: 90876666		=> 90876666
	// RRERRERR TREEX 3777-11**-****-3030 => 3777-11**-****-3030
	// TFT SINPE A: 3777-11**-****-3030$^1   => 3777-11**-****-3030$^1

	regex searchPhrase{ "([[:alnum:]-*$^]*)$" };

	smatch m;
	bool ok = regex_search(sdata, m, searchPhrase);



	string ssdata = "TFT SINPE A: 3777-11**-****-3030$^1";
	ok = regex_search(ssdata, m, searchPhrase);
	
	// TEF A:898877778 ==> take last word
	// loop accounts to find this word within account number
	// stop if found and make current selection
	// std::string nombre_concepto = 
	// auto pos = m_discriminator.find_last_of(" ");
	// auto search = m_discriminator.substr(pos + 1);

	auto x = m.str();
	x = m[1].str();
#endif

	if (m_discriminator.empty())	return;
	
	const regex searchPhrase{ "([[:alnum:]-*$^]*)$" };

	smatch m;
	bool ok = regex_search(m_discriminator, m, searchPhrase);

	auto x = m.str();

	if (x.empty())	return;

	auto lx = Util::to_cstring(x);

	for (auto index = 0u; index < m_listAccounts.GetCount(); ++index)
	{
		CString str;
		m_listAccounts.GetLBText(index, str);
		assert(str.GetLength() > 0);
		auto found = str.Find(lx);
		if (found != -1)
		{
			auto pk = m_listAccounts.GetItemData(index);
			m_accountsCB.select(pk);
			break;
		}
	}

	
}

void ConceptosDlg::OnBnClickedBBorrar()
{
	// TODO: Add your control notification handler code here
#if 0
	auto concepto = m_conceptosLB.current();

	if( ! concepto)
	{
		MessageBoxW(L"Falta escoger el concepto");
		return;
	}

	bool has_links = RecordLinks::has_links(*concepto);
	if (!has_links)
	{
		m_conceptosLB.delete_current_sel();
	}
#else
	m_conceptosLB.delete_current_sel();
#endif
}


void ConceptosDlg::OnBnClickedBAddAccount()
{
	// TODO: Add your control notification handler code here
	CuentaDlg dlg;
	dlg.DoModal();
	OnRefresh();
	// m_accountsCB.loadLB();
}


void ConceptosDlg::OnRefresh()
{
	m_accountsCB.loadLBOrderBy(&Account::number);
}

void ConceptosDlg::OnBnClickedBUpdateConcepto()
{
#if 0
	// TODO: Add your control notification handler code here
	std::optional<Concepto> concepto = m_conceptosLB.current();
	if (! concepto)
	{
		MessageBoxW(L"Falta escoger el concepto");
		return;
	}

	bool changes = false;
	CString rNombre;
	m_nombre.GetWindowTextW(rNombre);

	if (!rNombre.IsEmpty())
	{
		auto nombre = Util::from_cstring(rNombre);
		concepto->name = nombre;
		changes = true;
	}

	auto cuenta = m_accountsCB.current();
	if( cuenta)
	{
		concepto->fkey_account = cuenta->id_account;
		changes = true;
	}

	if (!changes)
	{
		MessageBox(L"No hay cambios en este objeto");
		return;
	}

	m_conceptosLB.update(*concepto);
	m_conceptosLB.loadLB();

	m_concepto = concepto;
#endif
}


void ConceptosDlg::OnLbnSelchangeLConceptos()
{
	// TODO: Add your control notification handler code here
	auto concepto = m_conceptosLB.current();

	m_nombre.SetWindowTextW(Util::to_cstring(concepto->name));
	m_accountsCB.select(concepto->fkey_account);
	m_id.SetWindowTextW(Util::to_cstring(concepto->id_concepto));
}


void ConceptosDlg::OnBnClickedBAplicarConcepto()
{
	// TODO: Add your control notification handler code here
	auto concepto = getCurrent<Concepto>(m_id);
	
	CString rNombre;
	m_nombre.GetWindowTextW(rNombre);

	auto nombre = Util::from_cstring(rNombre);
	if (rNombre.IsEmpty())
	{
		MessageBox(L"Falta escoger nombre");
		return;
	}

	auto cuenta = m_accountsCB.current();
	if (! cuenta)
	{
		MessageBox(L"Falta escoger cuenta");
		return;
	}

	if (!concepto)
	{
		auto whereClause = (c(&Concepto::name) == nombre.c_str());

		std::optional<Concepto> concepto_by_value = m_conceptosLB.exists(whereClause); // , & Concepto::id_concepto, & Concepto::name);

		if (concepto_by_value)
		{
			MessageBoxW(L"Concepto found for another primary key");
		}
		concepto = concepto_by_value;
	}
	if (!concepto)    // insert
	{
		concepto = m_conceptosLB.insert(nombre, cuenta->id_account);
		m_conceptosLB.insert_into_listbox(*concepto);
	}
	else               // update
	{
		concepto->name = nombre;
		concepto->fkey_account = cuenta->id_account;
		m_conceptosLB.update(*concepto);
	}
	m_conceptosLB.loadLBOrderBy (&Concepto::name);
	m_concepto = concepto;

	setIdFromRecord<Concepto>(m_id, concepto->id_concepto);
}


void ConceptosDlg::OnBnClickedCDesconocida()
{
	// TODO: Add your control notification handler code here

	auto& storage = Storage::getStorage();
	auto rows = storage.get_all<Account>(where(like (&Account::number, "%Desconocida%")));
	if( rows.size() ==0)
	{
		return;
	}

	auto account = rows[0];
	m_accountsCB.select(account.id_account);
	OnBnClickedBAplicarConcepto();
}
