#pragma once
#include <afxwin.h>

#include "boxContents.h"


// BancoDlg dialog

class BancoDlg : public CDialog
{
	DECLARE_DYNAMIC(BancoDlg)

	BoxContents<Banco, &Banco::id_bank> m_bancosLB;
	BoxContents<Pais, &Pais::id_pais, CComboBox> m_paisCB;
public:
	BancoDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~BancoDlg();

	std::optional<Banco> m_banco;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BancoDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_nombre_banco;
	CListBox m_list_paises;
	CListBox m_list_bancos;
	CComboBox m_paises;
public:
	afx_msg void OnBnClickedBAddPais();
	afx_msg void OnBnClickedBAplicarBanco();
private:
	CEdit m_ubicacion;
public:
	afx_msg void OnBnClickedBUpdateBanco();
	afx_msg void OnBnClickedBBorrar();
private:
	CEdit m_id_banco;
public:
	afx_msg void OnLbnSelchangeLBancos();
};
