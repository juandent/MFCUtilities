#include "pch.h"
#include "LocationGrid.h"
#include "data.h"
#include "..\ORM_Extensions/JoinedGridDisplayer.h"


int LocationGrid::get_location_id(int row) const
{
	auto location_id_cs = GetItemText(row, ID_COLUMN);
	auto location_id_s = Util::to_string(location_id_cs.GetBuffer());

	if (location_id_s.empty())	return -1;

	int location_id = std::stoi(location_id_s);
	return location_id;
}


int LocationGrid::row_for_id(int id) const
{
	const int count = GetRowCount();

	for( int row = 1; row < count; ++row )
	{
		int loc_id = this->get_location_id(row);
		if (loc_id == id) return row;
	}

	return -1;
}


