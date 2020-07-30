#pragma once

#include "Data_Tier.h"

#if 0
template<typename Table>
class DependentTable
{
public:
	DependentTable() {}

	template<typename WhereClause>
	bool existsLink(WhereClause& clause)
	{
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto e = storage.count<Table>(where(clause));
		if (e.size() > 0)
		{
			return true;
		}
		return false;
	}
};
#endif

#if 1	
	template<typename DependentTable, int DependentTable::* foreignKey, typename Table, int Table::*primaryKey >
	bool existsLink( Table record)
	{
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto count = storage.count<DependentTable>(where((c(&DependentTable::foreignKey) == (record.*primaryKey))));
		return count > 0;
	}
#endif



template<typename DependentTable, auto ForeignKey, typename Table, int Table::* primaryKey >
class LinkInfo
	{
	public:
		
	bool existsLink(Table record)
	{
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto count = storage.count<DependentTable>(where(ForeignKey == (record.*primaryKey)));
		return count > 0;
	}

	};