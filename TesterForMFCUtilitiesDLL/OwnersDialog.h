#pragma once


// OwnersDialog dialog

class OwnersDialog : public CDialog
{
	DECLARE_DYNAMIC(OwnersDialog)

public:
	OwnersDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OwnersDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OwnersDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CJDGridCtrl m_grid;
public:
	virtual BOOL OnInitDialog();
};
