#pragma once
#include "Model/Model.ORM.Definitions.h"
#include "GridCellLite.h"

struct Queries
{
	struct StatementLines_ForDate
	{
		template<typename where_type>
		static auto getRows(where_type where_clause)
		{
			using namespace sqlite_orm;
			using namespace Model;

			auto statement_line_flattened = ORM::storage
				.select(columns(&StatementLine::m_statement_date,
					&StatementLine::m_belongs_to_account_fid,
					&StatementLine::m_lineDate,
					&StatementLine::m_concept_fid,
					&StatementLine::m_amountInLocal,
					&StatementLine::m_amountInDollars,
					&StatementLine::m_enabled,
					&StatementLine::m_category_fid),
					//where(where_clause));
					where(c(&StatementLine::m_statement_date) == where_clause));
			return statement_line_flattened;
		}
		inline static std::vector<std::string> headers{ "STMT DATE", "ACCOUNT", "LINE DATE", "CONCEPT", "AMT LOCAL","AMT DOLLARS", "ENABLED?","CATEGORY" };
	};
	struct StatementLines_Joined_Account
	{
		static auto& getRows()
		{
			using namespace Model;
			using namespace sqlite_orm;

			static auto innerJoinRows = ORM::storage.select(
				columns(&StatementLine::m_lineDate, &StatementLine::m_amountInLocal,
					&StatementLine::m_belongs_to_account_fid, &Account::m_number_id),
				inner_join<Account>(on(c(&StatementLine::m_belongs_to_account_fid) == &Account::m_number_id)));
			return innerJoinRows;
		}
		inline static std::vector<std::string> headers{ "LINE DATE", "LOCAL AMOUNT", "BELONGS TO", "ACCOUNT ID" };
	};
};
struct Elements
{
	struct StatementLines_ForDate
	{
		using type = decltype(Queries::StatementLines_ForDate::getRows(std::declval<date::sys_days>())[0]);
	};
	struct StatementLines_Joined_Account
	{
		using type = decltype(Queries::StatementLines_Joined_Account::getRows()[0]);
	};
};
