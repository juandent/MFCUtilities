#pragma once

#include <JDGridCtrl.h>
#include "Data.h"
#include "..\ORM_Extensions/JoinedGridDisplayer.h"


struct IDisplayer;

class LocationGrid :    public CJDGridCtrl
{
    enum { ID_COLUMN = 1};

    std::unique_ptr<IDisplayer> m_displayer;
public:
    LocationGrid() {}

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



		long count = lines.size();
		auto strCount = Util::to_cstring(count);


		std::vector<std::string> headers{ "ID LOCATION", "NAME", "URL", "EMAIL" };

		m_displayer.reset(new JoinedGridDisplayer<decltype(lines[0]), IntegerList<>, IntegerList<>>(*this, std::move(lines), std::move(headers)));
		m_displayer->display();

    }

    int get_location_id(int row) const;
	int row_for_id(int id) const;
};

