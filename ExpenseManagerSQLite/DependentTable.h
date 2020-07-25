#pragma once

#include "Data_Tier.h"


template<typename Table>
class DependentTable
{
public:
	DependentTable() {}

	template<typename WhereClause, typename ...Cols>
	bool existsLink(WhereClause& clause, Cols&& ... cols)
	{
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto e = storage.select(columns(cols...), where(clause));
		if (e.size() > 0)
		{
			return true;
		}
		return false;
	}
};


#if 0	
	template<typename int Table::* foreignKey, typename ...Cols>
	bool existsLink(int primary_key, Cols...cols)
	{
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto vec = storage.select(columns(cols...), where(c(&Table::foreignKey) == primary_key));
		
	}
#endif


