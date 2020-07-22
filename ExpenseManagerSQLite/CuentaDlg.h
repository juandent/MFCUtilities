#pragma once
#include <afxwin.h>


// CuentaDlg dialog

class CuentaDlg : public CDialog
{
	DECLARE_DYNAMIC(CuentaDlg)

public:
	CuentaDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CuentaDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CuentaDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_id_cuenta;
	CEdit m_descripcion;
	CEdit m_numero;
	CComboBox m_dueno_combo;
public:
	afx_msg void OnBnClickedBAddDuenos();
private:
	CButton m_tarjeta;
	CListBox m_list_cuentas;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBAddBancos();
};
