// PasswordDlg.cpp : implementation file
//

#include "pch.h"
#include "Passwords.h"
#include "afxdialogex.h"
#include "PasswordDlg.h"
#include "..\ORM_Extensions/HandleSystemError.h"


// PasswordDlg dialog

IMPLEMENT_DYNAMIC(PasswordDlg, CDialog)

PasswordDlg::PasswordDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PasswordDlg, pParent),
	m_password_listLB(  m_password_list, [](const Password& password)
		{
			return Util::to_cstring(password.password);
		}),
	m_location_namesCB( m_location_names, [](const Location& location)
		{
			return Util::to_cstring(location.simple_dump());
		})

{

}

PasswordDlg::~PasswordDlg()
{
}

void PasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_LOCATION_NAMES, m_location_names);
	DDX_Control(pDX, IDC_L_PASSWORDS, m_password_list);
	DDX_Control(pDX, IDC_E_PASSWORD_ID, m_id);
	DDX_Control(pDX, IDC_E_PASSWORD, m_password);
	DDX_Control(pDX, IDC_DATETIME_PICKER, m_beginning_date);
}


BEGIN_MESSAGE_MAP(PasswordDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CB_LOCATION_NAMES, &PasswordDlg::OnCbnSelchangeCbLocationNames)
	ON_BN_CLICKED(ID_GRABAR, &PasswordDlg::OnBnClickedGrabar)
	ON_LBN_SELCHANGE(IDC_L_PASSWORDS, &PasswordDlg::OnLbnSelchangeLPasswords)
	ON_BN_CLICKED(ID_BORRAR, &PasswordDlg::OnBnClickedBorrar)
	ON_BN_CLICKED(ID_NUEVO, &PasswordDlg::OnBnClickedNuevo)
	ON_BN_CLICKED(ID_CERRAR, &PasswordDlg::OnBnClickedCerrar)
END_MESSAGE_MAP()


// PasswordDlg message handlers


BOOL PasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_location_namesCB.loadLBOrderBy(&Location::name);


	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void PasswordDlg::OnCbnSelchangeCbLocationNames()
{
	// TODO: Add your control notification handler code here
	auto location = m_location_namesCB.current();

	if (!location) return;

	auto where_clause = getPasswordWhereClauseNoAlias(location->id);

	m_password_listLB.loadLBOrderByDescWhere(&Password::begining_date, where_clause);
}


void PasswordDlg::OnBnClickedGrabar()
{
	// TODO: Add your control notification handler code here
	auto password = getCurrent<Password>(m_id);
	auto location = m_location_namesCB.current();

	if (!location)
	{
		MessageBox(L"Debe escoger location");
		return;
	}

	std::string name;
	m_password >> name;
	// = GetText(m_password);
	std::chrono::sys_days date;
	// = GetDate(m_beginning_date);
	m_beginning_date >> date;

	int id;
	// = GetLongLong(m_id);
	m_id >> id;
	auto fkey_location = location->id;

	if (name.empty())
	{
		MessageBoxW(L"No se puede dejar el password vacio");
		return;
	}

	try
	{
		if (!password)	// insert
		{
			password = m_password_listLB.insert(name, date, fkey_location);
			m_password_listLB.insert_into_listbox(*password);
		}
		else                // update
		{
			password->begining_date = date;
			password->fkey_location = fkey_location;
			password->password = name;
			m_password_listLB.update(*password);
		}
		auto where_clause = getPasswordWhereClauseNoAlias(location->id);

		m_password_listLB.loadLBOrderByDescWhere(&Password::begining_date, where_clause);
		setIdFromRecord<Password>(m_id, password->id);
	}
	catch (std::exception& exc)
	{
		handleApply(exc);
		OnLbnSelchangeLPasswords();
	}
}


void PasswordDlg::OnLbnSelchangeLPasswords()
{
	// TODO: Add your control notification handler code here
	auto password = m_password_listLB.current();

	if (!password)	return;


	m_id << password->id;
	// SetText(m_password, password->password);
	m_password << password->password;
	// SetDate(m_beginning_date, password->begining_date);
	m_beginning_date << password->begining_date;

	this->m_location_namesCB.select(password->fkey_location);
	m_password_listLB.select(password->id);
}


void PasswordDlg::OnBnClickedBorrar()
{
	// TODO: Add your control notification handler code here
	int id;
	// = GetLongLong(m_id);
	m_id >> id;
	m_password_listLB.select(id);
	if (m_password_listLB.delete_current_sel())
	{
		OnBnClickedNuevo();
	}
}




void PasswordDlg::OnBnClickedNuevo()
{
	// TODO: Add your control notification handler code here
	m_password_listLB.select(-1);
	//m_password_listLB.ResetContent();

	// SetText(m_id, ""s);
	m_id << ""s;
	// SetText(m_password, ""s);
	m_password << ""s;
	//this->m_location_namesCB.select(-1);
}


void PasswordDlg::OnBnClickedCerrar()
{
	// TODO: Add your control notification handler code here
	this->OnOK();
}
