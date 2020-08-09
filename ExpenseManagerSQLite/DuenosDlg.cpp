// DuenosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "DuenosDlg.h"
#include "afxdialogex.h"
#include "Data_Tier.h"
#include "RecordLinks.h"


// DuenosDlg dialog

IMPLEMENT_DYNAMIC(DuenosDlg, CDialog)

DuenosDlg::DuenosDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DuenosDlg, pParent),
	m_ownerLB{ m_list_duenos, [](AccountOwner& owner)
		{
				return JD::to_cstring(owner.name);
		} }
{

}

DuenosDlg::~DuenosDlg()
{
}

void DuenosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_NOMBRE, m_nombre);
	DDX_Control(pDX, IDC_L_DUENOS, m_list_duenos);
}


BEGIN_MESSAGE_MAP(DuenosDlg, CDialog)
	ON_BN_CLICKED(IDC_B_APPLY_OWNER, &DuenosDlg::OnBnClickedBApplyOwner)
	ON_BN_CLICKED(ID_B_BORRAR_OWNER, &DuenosDlg::OnBnClickedBBorrarOwner)
	ON_BN_CLICKED(IDC_B_UPDATE_OWNERS, &DuenosDlg::OnBnClickedBUpdateOwners)
	ON_LBN_SELCHANGE(IDC_L_DUENOS, &DuenosDlg::OnLbnSelchangeLDuenos)
END_MESSAGE_MAP()


// DuenosDlg message handlers


BOOL DuenosDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ownerLB.loadLB();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void DuenosDlg::OnBnClickedBApplyOwner()
{
	// TODO: Add your control notification handler code here
	CString rNombre;
	m_nombre.GetWindowTextW(rNombre);

	if (rNombre.IsEmpty() )
	{
		MessageBoxW(L"Falta el nombre del dueño");
		return;
	};

	auto nombre = JD::from_cstring(rNombre);

	auto whereClause = (c(&AccountOwner::id_owner) == nombre.c_str());

	std::optional<AccountOwner> account_owner = m_ownerLB.exists(whereClause, &AccountOwner::id_owner, &AccountOwner::name);

	if (!account_owner)	// new account
	{
		account_owner = m_ownerLB.insert(nombre);
		m_ownerLB.insert_into_listbox(*account_owner);
	}
	else            // update
 	{
		account_owner->name = nombre;
		m_ownerLB.update(*account_owner);
	}
	m_ownerLB.loadLB();
}


void DuenosDlg::OnBnClickedBBorrarOwner()
{
	// TODO: Add your control notification handler code here
#if 0
	auto owner = m_ownerLB.current();
	if( !owner)
	{
		MessageBoxW(L"Falta escoger dueño");
		return;
	}

	if (owner)
	{
		bool has_links = RecordLinks::has_links(*owner);
		if (!has_links)
		{
			m_ownerLB.delete_current_sel();
		}
	}
#else
	m_ownerLB.delete_current_sel();
#endif
}


void DuenosDlg::OnBnClickedBUpdateOwners()
{
#if 0
	// TODO: Add your control notification handler code here
	auto owner = m_ownerLB.current();
	if (!owner)
	{
		MessageBoxW(L"Falta escoger dueño");
		return;
	}

	CString rNombre;
	m_nombre.GetWindowTextW(rNombre);
	owner->name = JD::from_cstring(rNombre);

	m_ownerLB.update(*owner);
	m_ownerLB.loadLB();
#endif
}


void DuenosDlg::OnLbnSelchangeLDuenos()
{
	// TODO: Add your control notification handler code here
	auto owner = m_ownerLB.current();
	m_nombre.SetWindowTextW(JD::to_cstring(owner->name));
}
