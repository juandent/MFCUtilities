#pragma once

#include "RecordLinks.h"
#include "Data.h"

template<typename Table, int Table::* keyCol>
struct RefIntegrityManager
{
private:
	Storage::Storage_t& storage;
	bool canDelete(Table const& record)	// may throw std::exception
	{
		// if (record.*keyCol == -1)
		// {
		// 	throw std::exception{ "Primary key not set" };
		// }
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
		// TODO: we must ensure keyCol is not -1 in the caller! Remove this if...
		if (record.*keyCol == -1)	return;

		storage.update(record);
	}
public:
	RefIntegrityManager() : storage{ Storage::getStorage() } {}

	template<typename ...Cols>
	std::optional<Table> insert(Cols&&... cols)
	{
		Table record{ -1, cols... };
		if (!canInsertUpdate(record))
		{
			return std::nullopt;
		}
		record.*keyCol = storage.insert(record);
		return record;
	}
	void update(const Table& record)
	{
		// if (record.*keyCol == -1)	return;	// not persisted yet!

		if (canInsertUpdate(record))
		{
			storage.update(record);
		}
	}

	Table get(unsigned long long id)
	{
		Table record;
		try
		{
			record = storage.get<Table>(id);
		}
		catch (std::exception& exp)
		{
			// handle(exp, false);
			std::ostringstream ss;
			ss << "PK = " << id << " no existe en tabla " << storage.tablename<Table>();
			throw std::exception(ss.str().c_str());
			//			MessageBoxA(AfxGetMainWnd()->GetSafeHwnd(), ss.str().c_str(), "Falla en lectura", MB_OK);
		}
		return record;
	}

	bool remove(Table& record)
	{
		// try
		// {
		canDelete(record);

		storage.remove<Table>(get_pk(record));
		return true;
		// }
		// catch(std::exception& exp)
		// {
		// 	MessageBoxA(AfxGetMainWnd()->GetSafeHwnd(), exp.what(), "Falla en borrado", MB_OK);
		// }
	}

	int get_pk(Table& record)
	{
		return record.*keyCol;
	}
};

template<typename Table, int Table::* keyCol >
struct Accessor
{
private:
	Storage::Storage_t& storage;
public:
	Accessor() : storage{ Storage::getStorage() } {}

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
		return storage.get_all<Table>(order_by(clause).asc().collate_nocase());
	}

	template<typename orderByClause, typename whereClause>
	std::vector<Table> getAllOrderByWhere(orderByClause clause, whereClause where_clause)
	{
		using namespace sqlite_orm;
		return storage.get_all<Table>(where(where_clause), order_by(clause).asc().collate_nocase());
	}

	template<typename orderByClause, typename whereClause>
	std::vector<Table> getAllOrderByDescWhere(orderByClause clause, whereClause where_clause)
	{
		using namespace sqlite_orm;
		return storage.get_all<Table>(where(where_clause), order_by(clause).desc().collate_nocase());
	}

	template<typename orderByClause>
	std::vector<Table> getAllOrderByDesc(orderByClause clause)
	{
		using namespace sqlite_orm;
		return storage.get_all<Table>(order_by(clause).desc().collate_nocase());
	}



	template<typename WhereClause> //, typename ...Cols>
	std::optional<Table> exists(WhereClause& clause) //, Cols&& ... cols)
	{
		using namespace sqlite_orm;

		std::optional<Table> record;
		// auto e = storage.select(columns(cols...), where(clause));
		auto e = storage.select(columns(keyCol), where(clause));
		if (e.size() > 0)
		{
			auto id = std::get<0>(e[0]);
			record = storage.get<Table>(id);
		}

		return record;
	}

};
