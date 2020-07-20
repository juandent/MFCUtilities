#pragma once






template<typename Table, typename SearchType>
class TablePool
{
public:
	using Record = std::shared_ptr<Table>;
	using MapType = std::map<SearchType, Record>;
	using MapIterator = typename MapType::iterator;
private:
	MapType  pool;

	MapIterator lookupInPool(SearchType search_val);

	
public:
	TablePool() {}

	// look at the map first
	// if not there, look at the db
	// if not there, insert and make a std::shared_ptr<Table>
	// 

	void insert(SearchType search_val, const Record& record )
	{
		pool[search_val] = record;
	}

	template<typename WhereClause, typename ...Cols>
	Record find(Table record, SearchType search_val, WhereClause& clause, Cols&& ... cols)
	{
		MapIterator it = lookupInPool(search_val);
		if( it != pool.end())
		{
			return *it;
		}

		
		
		auto e = storage.select(columns(cols...), where(clause));
		if (e.size() > 0)
		{
			auto id = std::get<0>(e[0]);
			record = storage.get_pointer<Table>(id);
		}
		else
		{
			
		}
		
		return record;
	}

};
