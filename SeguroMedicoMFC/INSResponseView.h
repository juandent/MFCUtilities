#pragma once

#include "..\ExpenseManagerSQLite/IDisplayer.h"
#include "GridDisplayer.h"
#include "Data.h"
#include "TesterForMFCUtilitiesDLL/JoinedGridDisplayer.h"
#include "BoxContents.h"


class INSResponseDoc;

// INSResponseView form view

class INSResponseView : public CFormView
{
	DECLARE_DYNCREATE(INSResponseView)

	BoxContents<INSResponse, &INSResponse::id, CComboBox> m_insresponseCB;
	BoxContents<Doctor, &Doctor::id, CComboBox> m_doctoresCB;

protected:
	INSResponseView();           // protected constructor used by dynamic creation
	virtual ~INSResponseView();

	auto GetWhereStatement();
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSResponseView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// GetDocument() override();
	INSResponseDoc* GetDocument() const;

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Refresh();
	template <class T>
	void InitializeGridINSResponse(const T& t);
	template <class T>
	void InitializeGridINSResponseSummary(const T& t);
private:
	CJDGridCtrl m_grid_1;
	CJDGridCtrl m_grid_2;
	std::unique_ptr<IDisplayer> m_displayer_ins_response;
	std::unique_ptr<IDisplayer> m_displayer_ins_response_summary;
	CComboBox m_lista_ins_response;
public:
	afx_msg void OnBnClickedBFilter();
private:
	CComboBox m_lista_doctores;
};


