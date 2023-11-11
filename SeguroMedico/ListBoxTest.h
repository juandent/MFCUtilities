#pragma once
#include "afxdialogex.h"

#include "Data.h"

import box_contents;
import util;

// ListBoxTest dialog

inline auto& storage = Storage::getStorage();


class ListBoxTest : public CDialogEx
{
	DECLARE_DYNAMIC(ListBoxTest)

	BoxContents < Claim, &Claim::id, CListBox, Storage::Storage_t> m_box;
public:
	ListBoxTest(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ListBoxTest();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listbox;
	virtual BOOL OnInitDialog();
};
