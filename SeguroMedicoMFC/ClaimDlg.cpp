// ClaimDlg.cpp : implementation file
//

#include "pch.h"
#include "SeguroMedicoMFC.h"
#include "ClaimDlg.h"
#include "afxdialogex.h"


// ClaimDlg dialog

IMPLEMENT_DYNAMIC(ClaimDlg, CDialog)

ClaimDlg::ClaimDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ClaimDlg, pParent)
{

}

ClaimDlg::~ClaimDlg()
{
}

void ClaimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ClaimDlg, CDialog)
END_MESSAGE_MAP()


// ClaimDlg message handlers


BOOL ClaimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
