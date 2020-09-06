#pragma once

#include "RecordLinks.h"
#include "Data_Tier.h"

#if 0
template<typename Table>
struct RefIntegrity 
{
	bool canDelete()
	{
		return ! RecordLinks::has_links( static_cast<Table&>(*this));
	}
};
#endif

template<typename Table, int Table::* keyCol>
struct RefIntegrityManager
{
private:
	Storage::Storage_t& storage;
public:
	RefIntegrityManager() : storage{ Storage::getStorage() } {}

	bool canDelete(Table const& record)
	{
		if (record.*keyCol == -1) return false;
		return !RecordLinks::has_links(record);
	}
	bool canInsertUpdate(Table const& record)
	{
		return RecordLinks::foreignKeysExist(record);
	}
	template<typename ...Cols>
	Table insert(Cols&&... cols)
	{
		Table record{ -1, cols... };
		record.*keyCol = storage.insert(record);
		return record;
	}
	void update(const Table& record)
	{
		if (record.*keyCol == -1)	return;

		storage.update(record);
	}

	Table get(int id)
	{
		Table record;
		record = storage.get<Table>(id);
		return record;
	}

	bool remove(Table& record)
	{
		if (! canDelete(record)) 	return false;

		storage.remove<Table>(get_pk(record));
		return true;
	}

	int get_pk(Table& record)
	{
		return record.*keyCol;
	}

	std::vector<Table> getAll()
	{
		return storage.get_all<Table>();
	}
	template<typename whereClause>
	std::vector<Table> getAll(whereClause clause)
	{
		return storage.get_all<Table>(where(clause));
	}

	template<typename WhereClause, typename ...Cols>
	std::optional<Table> exists(WhereClause& clause, Cols&& ... cols)
	{
		std::optional<Table> record;
		auto e = storage.select(columns(cols...), where(clause));
		if (e.size() > 0)
		{
			auto id = std::get<0>(e[0]);
			record = storage.get<Table>(id);
		}

		return record;
	}

};
