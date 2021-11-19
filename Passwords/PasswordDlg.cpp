// PasswordDlg.cpp : implementation file
//

#include "pch.h"
#include "Passwords.h"
#include "afxdialogex.h"
#include "PasswordDlg.h"


// PasswordDlg dialog

IMPLEMENT_DYNAMIC(PasswordDlg, CDialog)

PasswordDlg::PasswordDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PasswordDlg, pParent),
	m_password_listLB(  m_password_list, [](Password& password)
		{
			return Util::to_cstring(password.password);
		}),
	m_location_namesCB( m_location_names, [](Location& location)
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
}


BEGIN_MESSAGE_MAP(PasswordDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CB_LOCATION_NAMES, &PasswordDlg::OnCbnSelchangeCbLocationNames)
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
