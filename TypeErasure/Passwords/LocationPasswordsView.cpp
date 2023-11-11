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
	: CFormView(IDD_LocationPasswordsView),
	m_search_subset{Storage::getStorage(), m_location_search}
{

}

LocationPasswordsView::~LocationPasswordsView()
{
}

void LocationPasswordsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_LOCATIONS, m_location_grid);
	DDX_Control(pDX, IDC_GRID_PASSWORDS, m_GridPasswords);
	DDX_Control(pDX, IDC_E_SEARCH_LOCATION, m_location_search);
	DDX_Control(pDX, IDC_E_ROWSELECTED, m_row_selected);
}

BEGIN_MESSAGE_MAP(LocationPasswordsView, CFormView)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_LOCATIONS, OnGrid1StartSelChange)
	ON_BN_CLICKED(IDC_B_LOCATION_DLG, &LocationPasswordsView::OnBnClickedBLocationDlg)
	ON_BN_CLICKED(IDC_B_PASSWORD_DLG, &LocationPasswordsView::OnBnClickedBPasswordDlg)
	ON_BN_CLICKED(IDC_B_UNIFY, &LocationPasswordsView::OnBnClickedBUnify)

	ON_BN_CLICKED(IDC_B_SEARCH_LOCATION, &LocationPasswordsView::OnBnClickedBSearchLocation)
	ON_EN_KILLFOCUS(IDC_E_SEARCH_LOCATION, &LocationPasswordsView::OnKillfocusESearchLocation)
	ON_BN_CLICKED(IDC_B_GETSELECTED, &LocationPasswordsView::OnBnClickedBGetselected)

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
	m_location_grid.Initialize(t);

#if 0
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
#endif
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

	auto location_id = m_location_grid.GetIdFromRow(row);

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

		//order_by(alias_column<als_p>(&Password::begining_date)).desc());
		multi_order_by(order_by(alias_column<als_p>(&Password::begining_date)).desc(), order_by(alias_column<als_p>(&Password::id)).desc()));



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
	dlg.DoModal();
	{
		this->InitializeGridLocations(true);
		this->InitializeGridPasswords(false);
		m_search_subset.clear();
	}
}

#include "PasswordDlg.h"

int LocationPasswordsView::get_location_id() const noexcept
{
	int id = m_location_grid.GetSelectedId();
	return id;
}

void LocationPasswordsView::OnBnClickedBPasswordDlg()
{
	// TODO: Add your control notification handler code here
	PasswordDlg dlg{};
	dlg.m_location_id = get_location_id();

	if (dlg.DoModal() == 1)
	{
		int location_id = get_location_id();
		if (location_id > -1 )
		{
			auto passwordWhere = getPasswordWhereClauseAlias(location_id);

			this->InitializeGridPasswords(passwordWhere);
		}
		else
		{
			this->InitializeGridPasswords(false);
		}
	}
}

//import StringLib;

void LocationPasswordsView::OnBnClickedBUnify()
{
	// TODO: Add your control notification handler code here
	//std::string one, two;
	//string_lib::compareStringsNoCase(one, two);

	Storage::upgrade_database();

}




void LocationPasswordsView::OnBnClickedBSearchLocation()
{

#if 1
	auto res = m_search_subset.get_next_result(&Location::name);
	if (!res)  return;
	auto loc = *res;

	const int row_to_select = m_location_grid.GetRowForId(loc.id);


	m_location_grid.SetSelectAndScroll(row_to_select);

	NM_GRIDVIEW pItem;
	pItem.iRow = row_to_select;
	pItem.iColumn = 0;

	NMHDR* pNM = reinterpret_cast<NMHDR*>(&pItem);

	OnGrid1StartSelChange(pNM, 0);


#else
	if (!m_found_locations.empty())
	{
		if (m_location_index + 1 >= m_found_locations.size() )
		{
			m_location_index = 0;
		}
		else
		{
			m_location_index++;
		}
		const auto loc = m_found_locations[m_location_index];

		m_location_search << loc.name;

		const int row_to_select = m_location_grid.GetRowForId(loc.id);

		// const int row_to_select = loc.id;

		m_location_grid.SetSelectAndScroll(row_to_select);

		NM_GRIDVIEW pItem;
		pItem.iRow = row_to_select;
		pItem.iColumn = 0;

		NMHDR* pNM = reinterpret_cast<NMHDR*>(&pItem);

		OnGrid1StartSelChange(pNM, 0);
	}
#endif
}


void LocationPasswordsView::OnKillfocusESearchLocation()
{
	//m_location_search.set_search_string();
	std::string location;
	m_location_search >> location;
	if(location.empty())
	{
		m_found_locations.clear();
		m_location_index = -1;
	}
	else
	{
		auto loc = Util::touppercase(location);

		auto search = "%" + loc + "%";

		auto location_where = like(&Location::name, search);

		m_search_subset.search(location_where, &Location::name);
		//auto found = Storage::getStorage().get_all<Location>(where(location_where), order_by(&Location::name));

		//m_found_locations = Storage::getStorage().get_all<Location>(where(location_where), order_by(&Location::name));

		//m_found_locations.swap(found);
		//m_location_index = -1;
	}
}




void LocationPasswordsView::OnBnClickedBGetselected()
{
	int selected_row = m_location_grid.GetSelectedMinRow();
	m_row_selected << selected_row;
}


