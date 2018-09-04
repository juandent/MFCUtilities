#pragma once

#include "afxwin.h"
#include <string>

#ifndef DLLS_H
#define MFC_UTILITIES_EXPORTS
#include <Dlls.h>
#endif

// CMessageDialog dialog

class MFC_UTILITIES_API CMessageDialog : public CDialog
{
	DECLARE_DYNAMIC(CMessageDialog)

public:
	explicit CMessageDialog(CWnd* pParent = NULL);   // standard constructor
	CMessageDialog(const std::string& msg, const std::string& title = "Error", CWnd* pParent = NULL);
	virtual ~CMessageDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESSAGE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_title;
	CString m_msgText;
	virtual BOOL OnInitDialog();
};
