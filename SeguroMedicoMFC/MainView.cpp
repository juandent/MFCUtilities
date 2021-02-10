// MainView.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "MainView.h"
#include "Data.h"
#include "GridDisplayer.h"
#include <tuple>
// #include "JoinedGridDisplayer.h"

// MainView

IMPLEMENT_DYNCREATE(MainView, CFormView)

MainView::MainView()
	: CFormView(IDD_MainView)
{

}

MainView::~MainView()
{
}

void MainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_1, m_grid_1);
}

BEGIN_MESSAGE_MAP(MainView, CFormView)
END_MESSAGE_MAP()


// MainView diagnostics

#ifdef _DEBUG
void MainView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MainView message handlers


void MainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	InitializeGrid();
	
}



void MainView::InitializeGrid()
{
	auto otherlines = Storage::getStorage().get_all<Patient>();

	std::vector<std::string> headers{ "ID", "FIRST NAME", "LAST NAME" };

	m_displayer_patients.reset( new GridDisplayer<Patient> (m_grid_1, std::move(otherlines), std::move(headers)));
	
	m_grid_1.m_sortingFunctions.push_back(Util::Comparison::Text);
	m_grid_1.m_sortingFunctions.push_back(Util::Comparison::Money);
	m_grid_1.m_sortingFunctions.push_back(Util::Comparison::Text);
	m_grid_1.m_sortingFunctions.push_back(Util::Comparison::Text);
	
	m_displayer_patients->display(&Patient::id, &Patient::first_name, &Patient::last_name);
}