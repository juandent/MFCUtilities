#include "stdafx.h"
#include "JoinedGridDisplayer.h"

#include "Model/Model.ORM.Definitions.h"

using namespace Model;
using namespace sqlite_orm;

void doITJoined()
{
	auto lines = ORM::storage.select(columns(&StatementLine::m_lineDate, &StatementLine::m_amountInLocal, &StatementLine::m_belongs_to_account_fid, &Account::m_number_id,
		&StatementLine::m_category_fid),
		inner_join<Account>(on(c(&StatementLine::m_belongs_to_account_fid) == &Account::m_number_id)));
	for (auto& row : lines)
	{
		auto line_date = std::get<0>(row);
		auto local_amount = std::get<1>(row);
		auto belongs_to_account = std::get<2>(row);
	}
	CJDGridCtrl grid;
	std::vector<std::string> headers{ "LINE DATE", "AMOUNT LOCAL", "BELONGS TO", "ACCOUNT ID", "CATEGORY FID" };
	JoinedGridDisplayer<decltype(lines[0])> displayer( grid, std::move(lines), std::move(headers) );
	displayer.display();
}