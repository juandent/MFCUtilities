// LocationPasswordsView.cpp : implementation file
//

#include "pch.h"
#include "Passwords.h"
#include "LocationPasswordsView.h"
#include "LocationDlg.h"

import Util;

// LocationPasswordsView

IMPLEMENT_DYNCREATE(LocationPasswordsView, CFormView)

LocationPasswordsView::LocationPasswordsView()
	: CFormView(IDD_LocationPasswordsView)
{

}

LocationPasswordsView::~LocationPasswordsView()
{
}

void LocationPasswordsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_LOCATIONS, m_GridLocations);
	DDX_Control(pDX, IDC_GRID_PASSWORDS, m_GridPasswords);
}

BEGIN_MESSAGE_MAP(LocationPasswordsView, CFormView)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_LOCATIONS, OnGrid1StartSelChange)
	ON_BN_CLICKED(IDC_B_LOCATION_DLG, &LocationPasswordsView::OnBnClickedBLocationDlg)
	ON_BN_CLICKED(IDC_B_PASSWORD_DLG, &LocationPasswordsView::OnBnClickedBPasswordDlg)
END_MESSAGE_MAP()


// LocationPasswordsView diagnostics

#ifdef _DEBUG
void LocationPasswordsView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void LocationPasswordsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

template<typename T>
void LocationPasswordsView::InitializeGridLocations(const T& t)
{

	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_l>(&Location::id),
		alias_column<als_l>(&Location::name),
		alias_column<als_l>(&Location::url),
		alias_column<als_l>(&Location::email)),

		where(t),

		order_by(alias_column<als_l>(&Location::name)).asc().collate_nocase());



	long count = otherlines.size();
	auto strCount = Util::to_cstring(count);


	std::vector<std::string> headers{ "ID LOCATION", "NAME", "URL", "EMAIL" };

	m_displayer_locations.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<>, IntegerList<>>(m_GridLocations, std::move(otherlines), std::move(headers)));
	m_displayer_locations->display();

}



// LocationPasswordsView message handlers


void LocationPasswordsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	this->InitializeGridLocations(true);
	// TODO: Add your specialized code here and/or call the base class
}

void LocationPasswordsView::OnGrid1StartSelChange(NMHDR* pNotifyStruct, LRESULT*)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	auto row = pItem->iRow;
	auto col = pItem->iColumn;

	if (row < 1) return;

	auto location_id_cs = m_GridLocations.GetItemText(row, 1);
	auto location_id_s = Util::to_string(location_id_cs.GetBuffer());
	auto location_id = std::stoi(location_id_s);

	auto passwordWhere = getPasswordWhereClauseAlias(location_id);

	InitializeGridPasswords(passwordWhere);
}

#if 0
void LocationPasswordsView::InitializeGridPasswords(int location_id)
{
	using namespace sqlite_orm;

	auto passwordWhere = (c(alias_column<als_p>(&Password::fkey_location)) == location_id);

	InitializeGridPasswords(passwordWhere);
}
#endif

template<typename T>
void LocationPasswordsView::InitializeGridPasswords(const T& t)
{

	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_p>(&Password::id),
		alias_column<als_p>(&Password::begining_date),
		alias_column<als_p>(&Password::password)),

		inner_join<als_l>(on(c(alias_column<als_p>(&Password::fkey_location)) == alias_column<als_l>(&Location::id))),

		where(t),

		order_by(alias_column<als_p>(&Password::begining_date)).desc());



	long count = otherlines.size();
	auto strCount = Util::to_cstring(count);


	std::vector<std::string> headers{ "ID PASSWORD", "BEGIN DATE", "PASSWORD" };

	m_displayer_passwords.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<>, IntegerList<>>(m_GridPasswords, std::move(otherlines), std::move(headers)));
	m_displayer_passwords->display();

}


void LocationPasswordsView::OnBnClickedBLocationDlg()
{
	// TODO: Add your control notification handler code here
	LocationDlg dlg{};
	if (dlg.DoModal() == 1)
	{
		this->InitializeGridLocations(true);
		this->InitializeGridPasswords(false);
	}
}

#include "PasswordDlg.h"

void LocationPasswordsView::OnBnClickedBPasswordDlg()
{
	// TODO: Add your control notification handler code here
	PasswordDlg dlg{};
	if (dlg.DoModal() == 1)
	{
		if (m_GridLocations.GetSelectedCount())
		{

			auto cell_range = m_GridLocations.GetSelectedCellRange();

			auto row = cell_range.GetMinRow();
			auto location_id_cs = m_GridLocations.GetItemText(row, 1);
			auto location_id_s = Util::to_string(location_id_cs.GetBuffer()); 
			auto location_id = std::stoi(location_id_s);
			auto passwordWhere = getPasswordWhereClauseAlias(location_id);

			this->InitializeGridPasswords(passwordWhere);
		}
		else
		{
			this->InitializeGridPasswords(false);
		}
	}
}
