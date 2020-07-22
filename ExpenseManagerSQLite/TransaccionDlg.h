#pragma once


// TransaccionDlg dialog

class TransaccionDlg : public CDialog
{
	DECLARE_DYNAMIC(TransaccionDlg)

public:
	TransaccionDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TransaccionDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TransaccionDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBAddStatement();
	afx_msg void OnBnClickedBAddConcept();
	afx_msg void OnBnClickedOk();
private:
	CMonthCalCtrl m_date_transaccion;
public:
	afx_msg void OnMcnSelchangeDateTransaction(NMHDR* pNMHDR, LRESULT* pResult);
};
