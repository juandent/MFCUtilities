#pragma once



// SubFormView form view

class SubFormView : public CFormView
{
	DECLARE_DYNCREATE(SubFormView)

protected:
	SubFormView();           // protected constructor used by dynamic creation
	virtual ~SubFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SubFormView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


