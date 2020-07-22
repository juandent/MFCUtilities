// CuentaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "CuentaDlg.h"
#include "afxdialogex.h"


// CuentaDlg dialog

IMPLEMENT_DYNAMIC(CuentaDlg, CDialog)

CuentaDlg::CuentaDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CuentaDlg, pParent)
{

}

CuentaDlg::~CuentaDlg()
{
}

void CuentaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID_CUENTA, m_id_cuenta);
	DDX_Control(pDX, IDC_E_DESCRIPTION, m_descripcion);
	DDX_Control(pDX, IDC_E_NUMERO, m_numero);
	DDX_Control(pDX, IDC_C_DUENO, m_dueno_combo);
	DDX_Control(pDX, IDC_R_TARJETA, m_tarjeta);
	DDX_Control(pDX, IDC_L_CUENTAS, m_list_cuentas);
}


BEGIN_MESSAGE_MAP(CuentaDlg, CDialog)
	ON_BN_CLICKED(IDC_B_ADD_DUENOS, &CuentaDlg::OnBnClickedBAddDuenos)
	ON_BN_CLICKED(IDOK, &CuentaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_B_ADD_BANCOS, &CuentaDlg::OnBnClickedBAddBancos)
END_MESSAGE_MAP()


// CuentaDlg message handlers


BOOL CuentaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CuentaDlg::OnBnClickedBAddDuenos()
{
	// TODO: Add your control notification handler code here
}


void CuentaDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CuentaDlg::OnBnClickedBAddBancos()
{
	// TODO: Add your control notification handler code here
}
