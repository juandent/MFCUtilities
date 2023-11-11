#pragma once

#include "RecordLinks.h"
#include "Data_Tier.h"


template<typename Table, int Table::* keyCol>
struct RefIntegrityManager
{
private:
	Storage::Storage_t& storage;

	bool canDelete(Table const& record)
	{
		if (record.*keyCol == -1) return false;
		return !RecordLinks::has_links(record);
	}
	bool canInsertUpdate(Table const& record)
	{
		return RecordLinks::foreignKeysExist(record);
	}

	///
	template<typename ...Cols>
	Table do_insert(Cols&&... cols)
	{
		Table record{ -1, cols... };
		record.*keyCol = storage.insert(record);
		return record;
	}
	void do_update(const Table& record)
	{
		if (record.*keyCol == -1)	return;

		storage.update(record);
	}
public:
	RefIntegrityManager() : storage{ Storage::getStorage() } {}

	template<typename ...Cols>
	std::optional<Table> insert(Cols&&... cols)
	{
		Table record{ -1, cols... };
		if( !canInsertUpdate(record))
		{
			return std::nullopt;
		}
		record.*keyCol = storage.insert(record);
		return record;
	}
	void update(const Table& record)
	{
		if (record.*keyCol == -1)	return;	// not persisted yet!

		if (canInsertUpdate(record))
		{
			storage.update(record);
		}
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
		using namespace sqlite_orm;
		return storage.get_all<Table>(where(clause));
	}

	template<typename orderByClause>
	std::vector<Table> getAllOrderBy(orderByClause clause)
	{
		using namespace sqlite_orm;
		return storage.get_all<Table>(order_by(clause));
	}

	// first column must be primary key!!
	template<typename WhereClause>  //, typename ...Cols>
	std::optional<Table> exists(WhereClause& clause)  //, Cols&& ... cols)
	{
		using namespace sqlite_orm;
		
		std::optional<Table> record;
		auto e = storage.select(columns(keyCol), where(clause));
		if (e.size() > 0)
		{
			auto id = std::get<0>(e[0]);
			record = storage.get<Table>(id);
		}

		return record;
	}

};
