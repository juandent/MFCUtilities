
// TesterForMFCUtilitiesDLLView.h : interface of the CTesterForMFCUtilitiesDLLView class
//

#pragma once


//namespace View
//{

//class BasicCategoryGridController;
//#include "BasicCategoryGridController.h"
#include "GridDisplayer.h"
#include "JoinedGridDisplayer.h"

#include "../Model/Model.ORM.Definitions.h"


#include "Queries.h"

	class CTesterForMFCUtilitiesDLLView : public CFormView
	{
		using category_grid_t = GridDisplayer<Model::Category>;
		using statement_lines_displayer_t = JoinedGridDisplayer<Elements::StatementLines_ForDate::type>;

		// order matters!!
		CJDGridCtrl m_Category_grid;
		std::unique_ptr<GridDisplayer<Model::Category>> m_Cat_grid_Displayer{nullptr};
		CJDGridCtrl m_statement_lines_grid;
		std::unique_ptr<statement_lines_displayer_t > m_statement_lines_displayer;


	protected: // create from serialization only
		CTesterForMFCUtilitiesDLLView() noexcept;
		DECLARE_DYNCREATE(CTesterForMFCUtilitiesDLLView)

	public:
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_TESTERFORMFCUTILITIESDLL_FORM };
#endif

		// Attributes
	public:
		CTesterForMFCUtilitiesDLLDoc* GetDocument() const;

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
		afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBnClickedButtonFilter();
	};

#ifndef _DEBUG  // debug version in TesterForMFCUtilitiesDLLView.cpp
	inline Controller::CTesterForMFCUtilitiesDLLDoc* CTesterForMFCUtilitiesDLLView::GetDocument() const
	{
		return reinterpret_cast<Controller::CTesterForMFCUtilitiesDLLDoc*>(m_pDocument);
	}
#endif

//}
