// BancoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "BancoDlg.h"
#include "afxdialogex.h"


// BancoDlg dialog

IMPLEMENT_DYNAMIC(BancoDlg, CDialog)

BancoDlg::BancoDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BancoDlg, pParent)
{

}

BancoDlg::~BancoDlg()
{
}

void BancoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_NOMBRE_BANCO, m_nombre_banco);
	DDX_Control(pDX, IDC_L_BANCOS, m_list_bancos);
	DDX_Control(pDX, IDC_C_PAIS, m_paises);
}


BEGIN_MESSAGE_MAP(BancoDlg, CDialog)
END_MESSAGE_MAP()


// BancoDlg message handlers


BOOL BancoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
