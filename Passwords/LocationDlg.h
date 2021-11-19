#pragma once
#include "afxdialogex.h"

#include "BoxContents.h"
#include "Data.h"

// LocationDlg dialog

class LocationDlg : public CDialog
{
	DECLARE_DYNAMIC(LocationDlg)

	BoxContents<Location, &Location::id> m_list_all_locationsLB;

public:
	LocationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~LocationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LocationDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_email;
	CEdit m_id;
	CEdit m_name;
	CEdit m_url;
	CListBox m_list_locations;

public:
	afx_msg void OnSelchangeListLocations();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedErase();
	afx_msg void OnBnClickedNew();
};
