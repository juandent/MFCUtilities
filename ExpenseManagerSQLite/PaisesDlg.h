#pragma once


// PaisesDlg dialog

class PaisesDlg : public CDialog
{
	DECLARE_DYNAMIC(PaisesDlg)

public:
	PaisesDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PaisesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PaisesDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CListBox m_list_paises;
public:
	afx_msg void OnBnClickedOk();
private:
	CEdit m_id_pais;
};
