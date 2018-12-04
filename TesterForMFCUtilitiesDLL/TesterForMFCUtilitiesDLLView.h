
// TesterForMFCUtilitiesDLLView.h : interface of the CTesterForMFCUtilitiesDLLView class
//

#pragma once

#include <JDGridCtrl.h>

//namespace View
//{

//class BasicCategoryGridController;
#include "BasicCategoryGridController.h"



	class CTesterForMFCUtilitiesDLLView : public CFormView
	{
		// order matters!!
		CJDGridCtrl m_myFirstGridFromADLL;
		std::unique_ptr<BasicCategoryGridController> m_pCatGridController;

	protected: // create from serialization only
		CTesterForMFCUtilitiesDLLView() noexcept;
		DECLARE_DYNCREATE(CTesterForMFCUtilitiesDLLView)

	public:
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_TESTERFORMFCUTILITIESDLL_FORM };
#endif

		// Attributes
	public:
		Controller::CTesterForMFCUtilitiesDLLDoc* GetDocument() const;

		// Operations
	public:

		// Overrides
	public:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual void OnInitialUpdate(); // called first time after construct
		virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
		virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
		virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
		virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

		// Implementation
	public:
		virtual ~CTesterForMFCUtilitiesDLLView();
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif

	protected:

		// Generated message map functions
	protected:
		afx_msg void OnFilePrintPreview();
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		DECLARE_MESSAGE_MAP()
	public:
	};

#ifndef _DEBUG  // debug version in TesterForMFCUtilitiesDLLView.cpp
	inline CTesterForMFCUtilitiesDLLDoc* CTesterForMFCUtilitiesDLLView::GetDocument() const
	{
		return reinterpret_cast<CTesterForMFCUtilitiesDLLDoc*>(m_pDocument);
	}
#endif

//}
