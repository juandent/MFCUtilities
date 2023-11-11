#pragma once
#include "afxdialogex.h"

// #include "..\ORM_Extensions/BoxContents.h"
import box_contents;

#include "Data.h"

// PasswordDlg dialog

class PasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(PasswordDlg)

	BoxContents<Location, &Location::id, CComboBox, Storage::Storage_t> m_location_namesCB;
	BoxContents<Password, &Password::id, CListBox, Storage::Storage_t> m_password_listLB;

public:
	PasswordDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PasswordDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PasswordDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CComboBox m_location_names;
	CListBox m_password_list;
public:
	afx_msg void OnCbnSelchangeCbLocationNames();
private:
	CEdit m_id;
	CEdit m_password;
	CDateTimeCtrl m_beginning_date;
public:
	afx_msg void OnBnClickedGrabar();
	afx_msg void OnLbnSelchangeLPasswords();
	afx_msg void OnBnClickedBorrar();
	afx_msg void OnBnClickedNuevo();
	afx_msg void OnBnClickedCerrar();
	int m_location_id = -1;
};
