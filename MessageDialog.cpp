// MessageDialog.cpp : implementation file
//

#include "stdafx.h"

#include <FixedPoint/arithmetic_types.h>

#include "MessageDialog.h"

#include "Util.h"
#include "JDMFCLibrary/Resource.h"


// import Util;

// CMessageDialog dialog

IMPLEMENT_DYNAMIC(CMessageDialog, CDialog)

CMessageDialog::CMessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MESSAGE_DIALOG, pParent)
	, m_msgText(_T("")), m_title{L"Error"}
{

}

CMessageDialog::CMessageDialog(const std::string & msg, const std::string& title, CWnd * pParent)
	: CDialog(IDD_MESSAGE_DIALOG, pParent), m_title{ Util::to_cstring(title)}
{
	m_msgText = Util::to_cstring(msg);
}

CMessageDialog::~CMessageDialog()
{
}

void CMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MSG, m_msgText);
}


BEGIN_MESSAGE_MAP(CMessageDialog, CDialog)
END_MESSAGE_MAP()


// CMessageDialog message handlers


BOOL CMessageDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	this->SetWindowTextW(m_title);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
