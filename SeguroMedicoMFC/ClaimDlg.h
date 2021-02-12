#pragma once


// ClaimDlg dialog

class ClaimDlg : public CDialog
{
	DECLARE_DYNAMIC(ClaimDlg)

public:
	ClaimDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ClaimDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ClaimDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
