#include "stdafx.h"
#include "StatementLineAdapter.h"
#include "Data_Tier.h"
#include "StatementLineGridController.h"
#include "TablePool.h"

StatementLineAdapter::StatementLineAdapter()
{
}

#if 0
	using namespace sqlite_orm;
	
	auto& storage = Storage::getStorage();


	// Scenario : Search object
	//
	// create pool object
	//
	TablePool<Statement, date::sys_days> statement_pool;
	const date::sys_days search_val = t.statement_date;
	// 1st: is object in pool?
	std::shared_ptr<Statement> obj = statement_pool.find(search_val);
	if( obj)
	{
		return obj;
	}
	else
	{
		//auto whereClause = c(&Statement::date) == t.statement_date;

		auto r = storage.select(columns(&Statement::id_statement, &Statement::date), where(c(&Statement::date) == t.statement_date));
		if( r.size() > 0)
		{
			int id = std::get<0>(r[0]);
			std::shared_ptr<Statement> pStatement = storage.get_pointer<Statement>(id);
			statement_pool[t.statement_date] = pStatement;
			return pStatement;
		}
		else
		{
			Statement statement{ -1, t.statement_date };
			statement.id_statement = storage.insert(statement);
			return statement_pool.insert(search_val, statement);
		}
	}
	
	

	StatementLine line;
	line.date = t.line_date;

	Concepto concepto;
	concepto;
#endif




template<typename Table, typename whereClause, typename ...Cols>
std::shared_ptr<Table> getFromDb()
{
	
}


void StatementLineAdapter::adapt(const Translation& t)
{
	using namespace sqlite_orm;

	auto& storage = Storage::getStorage();

	//m_statement_pool.find(t.statement_date);


	
	// Scenario : Search object
	//
	// create pool object
	//
	TablePool<Statement, date::sys_days> statement_pool;
	const date::sys_days search_val = t.statement_date;
	// 1st: is object in pool?
	std::shared_ptr<Statement> obj;// = statement_pool.find(search_val);
	if (obj)
	{
		;
	}
	else
	{
		auto whereClause = c(&Statement::date) == t.statement_date;
		auto columnClause = columns(&Statement::id_statement, &Statement::date);

		auto r = storage.select(columnClause, where(whereClause));

	//	auto r = storage.select(columns(&Statement::id_statement, &Statement::date), where(c(&Statement::date) == t.statement_date));
		if (r.size() > 0)
		{
			int id = std::get<0>(r[0]);
			std::shared_ptr<Statement> pStatement = storage.get_pointer<Statement>(id);
			//statement_pool[t.statement_date] = pStatement;
		}
		else
		{
			Statement statement{ -1, t.statement_date };
			statement.id_statement = storage.insert(statement);
			//statement_pool.insert(search_val, statement);
		}
	}

}


