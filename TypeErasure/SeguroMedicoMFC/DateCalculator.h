#pragma once
#include "afxdialogex.h"

#include <afxdtctl.h>

// DateCalculator dialog

class DateCalculator : public CDialogEx
{
	DECLARE_DYNAMIC(DateCalculator)

public:
	DateCalculator(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DateCalculator();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DateCalculator };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CDateTimeCtrl m_start_date;
	CDateTimeCtrl m_end_date;
	CEdit m_weeks;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedApply();
	afx_msg void OnBnClickedUp();
};
