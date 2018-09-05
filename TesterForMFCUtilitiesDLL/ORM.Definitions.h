#pragma once

#include "DataTier.h"


namespace ORM
{
	using namespace sqlite_orm;
	using namespace date;
	using DataTier::Account;
	using DataTier::Category;
	using DataTier::StatementLine;

	inline auto& getStorage()
	{
		static int flag = 0;
		static auto storage = make_storage("statements.sqlite",
			make_table("CATEGORY",
				make_column("name",
					&Category::m_name,
					primary_key()),
				make_column("is_real",
					&Category::m_real_expense_or_income)),
			make_table("ACCOUNT",
				make_column("number",
					&Account::m_number,
					primary_key()),
				make_column("currency",
					&Account::m_currency)),
			make_table("STATEMENTLINE",
				make_column("id",
					&StatementLine::m_id,
					primary_key()),
				make_column("number",
					&StatementLine::m_account),
				make_column("line_date",
					&StatementLine::m_lineDate),
				make_column("concept",
					&StatementLine::m_concept),
				make_column("colones",
					&StatementLine::m_amountInLocal),
				make_column("dolares",
					&StatementLine::m_amountInDollars),
				make_column("wide",
					&StatementLine::m_wide),
				//make_column("category",
					//&StatementLine::m_category),
				foreign_key(&StatementLine::m_account).references(&Account::m_number)));

		if (flag == 0)
		{
			flag = 1;
			storage.sync_schema();
		}
		return storage;
	}
	void fillDB();
}
