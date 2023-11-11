// DateCalculator.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "afxdialogex.h"
#include "DateCalculator.h"
#include "Data.h"

#include "ORM_Utilities-old.h"


// DateCalculator dialog

IMPLEMENT_DYNAMIC(DateCalculator, CDialogEx)

DateCalculator::DateCalculator(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DateCalculator, pParent)
{

}

DateCalculator::~DateCalculator()
{
}

void DateCalculator::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_DATE, m_start_date);
	DDX_Control(pDX, IDC_END_DATE, m_end_date);
	DDX_Control(pDX, IDC_E_WEEKS, m_weeks);
}


BEGIN_MESSAGE_MAP(DateCalculator, CDialogEx)
	ON_BN_CLICKED(ID_APPLY, &DateCalculator::OnClickedApply)
	ON_BN_CLICKED(ID_UP, &DateCalculator::OnBnClickedUp)
END_MESSAGE_MAP()


// DateCalculator message handlers



BOOL DateCalculator::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	std::chrono::sys_days date = GetDate(m_start_date);
	SetAmount(m_weeks, 10);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void DateCalculator::OnClickedApply()
{
	using namespace std::chrono;
	using namespace std::chrono_literals;

	// TODO: Add your control notification handler code here
	int week_count;
	m_weeks >> week_count;
	//auto week_count = GetInteger(m_weeks);

	std::chrono::sys_days date = GetDate(m_start_date);

	weeks w{ week_count };

	date += w;

	SetDate(m_end_date, date);
}


void DateCalculator::OnBnClickedUp()
{
	// TODO: Add your control notification handler code here
	std::chrono::sys_days date = GetDate(m_end_date);
	SetDate(m_start_date, date);
}
