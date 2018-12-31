#pragma once

#include "GridDisplayer.h"
//#include "JoinedGridDisplayer.h"

#include "../Model/Model.ORM.Definitions.h"


#if 0
struct Selects
{
	struct OwnersView
	{
		static auto getOwners()
		{
			using namespace sqlite_orm;
			using namespace Model;

			auto owners_data = ORM::storage
				.select(columns(&Person::m_id,
					&Person::m_first_name,
					&Person::m_last_name,
					&Person::m_company_name));
			return owners_data;
		}
		inline static std::vector<std::string> headers{ "PERS ID", "FIRST NAME", "LAST NAME", "COMPANY"};
	};
#endif

// OwnersView view

class OwnersView : public CFormView
{

	// order matters!!
	CJDGridCtrl m_grid;
	std::unique_ptr<GridDisplayer<Model::Person>> m_grid_Displayer{ nullptr };


	DECLARE_DYNCREATE(OwnersView)

protected:
	OwnersView();           // protected constructor used by dynamic creation
	virtual ~OwnersView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_OWNERSVIEW_FORM };
	CDocument* GetDocument() const;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	virtual void DoDataExchange(CDataExchange* pDC) override;
	virtual void OnInitialUpdate() override;
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	virtual void OnBeginPrinting(CDC*pDC, CPrintInfo* pInfo) override;
	virtual void OnEndPrinting(CDC*pDC, CPrintInfo* pInfo) override;
	virtual void OnPrint(CDC*pDC, CPrintInfo* pInfo) override;
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT flags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
};


