// OwnersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TesterForMFCUtilitiesDLL.h"
#include "OwnersDialog.h"
#include "afxdialogex.h"


// OwnersDialog dialog

IMPLEMENT_DYNAMIC(OwnersDialog, CDialog)

OwnersDialog::OwnersDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_OwnersDialog, pParent)
{

}

OwnersDialog::~OwnersDialog()
{
}

void OwnersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_DIALOG, m_grid);
}


BEGIN_MESSAGE_MAP(OwnersDialog, CDialog)
END_MESSAGE_MAP()


// OwnersDialog message handlers


BOOL OwnersDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_grid.SetColumnCount(4);
	m_grid.SetRowCount(10);
	m_grid.SetHeaderSort(true);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetFixedRowCount(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
