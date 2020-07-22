// PaisesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "PaisesDlg.h"
#include "afxdialogex.h"


// PaisesDlg dialog

IMPLEMENT_DYNAMIC(PaisesDlg, CDialog)

PaisesDlg::PaisesDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PaisesDlg, pParent)
{

}

PaisesDlg::~PaisesDlg()
{
}

void PaisesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_L_PAISES, m_list_paises);
	DDX_Control(pDX, IDC_E_ID_PAIS, m_id_pais);
}


BEGIN_MESSAGE_MAP(PaisesDlg, CDialog)
	ON_BN_CLICKED(IDOK, &PaisesDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// PaisesDlg message handlers


BOOL PaisesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void PaisesDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
