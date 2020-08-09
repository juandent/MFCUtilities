#pragma once
#include "BoxContents.h"


// DuenosDlg dialog

class DuenosDlg : public CDialog
{
	DECLARE_DYNAMIC(DuenosDlg)
	BoxContents<AccountOwner, &AccountOwner::id_owner> m_ownerLB;
public:
	DuenosDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DuenosDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DuenosDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBApplyOwner();
private:
	CEdit m_nombre;
	CListBox m_list_duenos;
public:
	afx_msg void OnBnClickedBBorrarOwner();
	afx_msg void OnBnClickedBUpdateOwners();
	afx_msg void OnLbnSelchangeLDuenos();
private:
	CEdit m_id_owner;
};
