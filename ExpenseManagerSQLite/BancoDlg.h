#pragma once
#include <afxwin.h>


// BancoDlg dialog

class BancoDlg : public CDialog
{
	DECLARE_DYNAMIC(BancoDlg)

public:
	BancoDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~BancoDlg();

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
};
