
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

	

	struct Selects
	{
		struct StatementLineFlat
		{
			static auto getStatementLineFlat(date::sys_days statement_date)
			{
				using namespace sqlite_orm;
				using namespace Model;

				auto statement_line_flattened = ORM::storage
					.select(columns(&StatementLine::m_statement_date,
						&StatementLine::m_belongs_to_account_fid,
						&StatementLine::m_lineDate,
						&StatementLine::m_concept_fid,
						&StatementLine::m_amountInLocal,
						&StatementLine::m_amountInDollars,
						&StatementLine::m_enabled,
						&StatementLine::m_category_fid), where(c(&StatementLine::m_statement_date) == statement_date));
				return statement_line_flattened;
			}
			inline static std::vector<std::string> headers{ "STMT DATE", "ACCOUNT", "LINE DATE", "CONCEPT", "AMT LOCAL","AMT DOLLARS", "ENABLED?","CATEGORY" };
		};
		struct StatementLine_Joined_Account
		{
			static auto& getContainer()
			{
				using namespace Model;
				using namespace sqlite_orm;

				static auto innerJoinRows = ORM::storage.select(
					columns(&StatementLine::m_lineDate, &StatementLine::m_amountInLocal,
						&StatementLine::m_belongs_to_account_fid, &Account::m_number_id),
					inner_join<Account>(on(c(&StatementLine::m_belongs_to_account_fid) == &Account::m_number_id)));
				return innerJoinRows;
			}
			inline static std::vector<std::string> headers{ "LINE DATE", "LOCAL AMOUNT", "BELONGS TO", "ACCOUNT ID" }; 
		};
	};


	class CTesterForMFCUtilitiesDLLView : public CFormView
	{
		//using Element = decltype(Selects::StatementLine_Joined_Account::getContainer()[0]);
		const date::sys_days last = std::chrono::time_point_cast<date::days>(std::chrono::system_clock::now());
		using Element = decltype(Selects::StatementLineFlat::getStatementLineFlat(last)[0]);

		// order matters!!
		CJDGridCtrl m_Category_grid;
		std::unique_ptr<GridDisplayer<Model::Category>> m_Cat_grid_Displayer{nullptr};
		CJDGridCtrl m_statement_lines_grid;
		std::unique_ptr<JoinedGridDisplayer<Element>> m_statement_lines_grid_Displayer{nullptr};


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
		afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
		DECLARE_MESSAGE_MAP()
	public:
	};

#ifndef _DEBUG  // debug version in TesterForMFCUtilitiesDLLView.cpp
	inline Controller::CTesterForMFCUtilitiesDLLDoc* CTesterForMFCUtilitiesDLLView::GetDocument() const
	{
		return reinterpret_cast<Controller::CTesterForMFCUtilitiesDLLDoc*>(m_pDocument);
	}
#endif

//}
