#pragma once

#include <JDGridCtrl.h>
#include "Data.h"
#include "..\ORM_Extensions/JoinedGridDisplayer.h"



template<int ID_COLUMN = 1>
class GridId
{
	CJDGridCtrl& m_grid;
public:
	GridId(CJDGridCtrl& grid) : m_grid{ grid } {}
	int GetId(const int row) const
	{
		auto id_cs = m_grid.GetItemText(row, ID_COLUMN);
		auto id_s = Util::to_string(id_cs.GetBuffer());

		if (id_s.empty())	return -1;

		int id = std::stoi(id_s);
		return id;
	}
	int GetRowForId(const int id) const
	{
		const int count = m_grid.GetRowCount();

		for (int row = 1; row < count; ++row)
		{
			int row_id = this->GetId(row);
			if (row_id == id) return row;
		}

		return -1;
	}
};


struct IDisplayer;

class LocationGrid :    public CJDGridCtrl
{
	const GridId<> m_grid_id;
    std::unique_ptr<IDisplayer> m_displayer;
public:
	LocationGrid() : m_grid_id{*this} {}

	template<typename T>
	void Initialize(T t)
    {
		auto lines = Storage::getStorage().select(columns(
			alias_column<als_l>(&Location::id),
			alias_column<als_l>(&Location::name),
			alias_column<als_l>(&Location::url),
			alias_column<als_l>(&Location::email)),

			where(t),
			order_by(alias_column<als_l>(&Location::name)).asc().collate_nocase());


		std::vector<std::string> headers{ "ID LOCATION", "NAME", "URL", "EMAIL" };

		m_displayer.reset(new JoinedGridDisplayer<decltype(lines[0]), IntegerList<>, IntegerList<>>(*this, std::move(lines), std::move(headers)));
		m_displayer->display();

    }

	int GetId(int row) const { return m_grid_id.GetId(row); }
	int GetRowForId(int id) const { return m_grid_id.GetRowForId(id); }
    // int get_location_id(int row) const;
	// int row_for_id(int id) const;
};


