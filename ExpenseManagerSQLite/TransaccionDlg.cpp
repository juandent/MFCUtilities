// TransaccionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "TransaccionDlg.h"
#include "afxdialogex.h"


// TransaccionDlg dialog

IMPLEMENT_DYNAMIC(TransaccionDlg, CDialog)

TransaccionDlg::TransaccionDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TransaccionDlg, pParent)
{

}

TransaccionDlg::~TransaccionDlg()
{
}

void TransaccionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_TRANSACTION, m_date_transaccion);
}


BEGIN_MESSAGE_MAP(TransaccionDlg, CDialog)
	ON_BN_CLICKED(IDC_B_ADD_STATEMENT, &TransaccionDlg::OnBnClickedBAddStatement)
	ON_BN_CLICKED(IDC_B_ADD_CONCEPT, &TransaccionDlg::OnBnClickedBAddConcept)
	ON_BN_CLICKED(IDOK, &TransaccionDlg::OnBnClickedOk)
	ON_NOTIFY(MCN_SELCHANGE, IDC_DATE_TRANSACTION, &TransaccionDlg::OnMcnSelchangeDateTransaction)
END_MESSAGE_MAP()


// TransaccionDlg message handlers


BOOL TransaccionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void TransaccionDlg::OnBnClickedBAddStatement()
{
	// TODO: Add your control notification handler code here
}


void TransaccionDlg::OnBnClickedBAddConcept()
{
	// TODO: Add your control notification handler code here
}


void TransaccionDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void TransaccionDlg::OnMcnSelchangeDateTransaction(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
