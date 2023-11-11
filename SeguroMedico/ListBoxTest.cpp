// ListBoxTest.cpp : implementation file
//

#include "pch.h"
#include "SeguroMedico.h"
#include "afxdialogex.h"
#include "ListBoxTest.h"

// ListBoxTest dialog

IMPLEMENT_DYNAMIC(ListBoxTest, CDialogEx)

ListBoxTest::ListBoxTest(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent),
	m_box(m_listbox, [](const Claim& claim)
	{
			return util::to_cstring(claim.simple_dump());
	}, storage)
{
}

ListBoxTest::~ListBoxTest()
{
}

void ListBoxTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTTEST, m_listbox);
}


BEGIN_MESSAGE_MAP(ListBoxTest, CDialogEx)
END_MESSAGE_MAP()


// ListBoxTest message handlers


BOOL ListBoxTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_box.loadBox(c(&Claim::amount) < 10000, order_by(&Claim::amount).desc(), order_by(&Claim::id));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
