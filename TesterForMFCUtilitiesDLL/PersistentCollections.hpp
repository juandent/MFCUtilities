#pragma once
#include "DataTier.h"


class PersistentCollections
{
public:
	static std::vector<std::shared_ptr<DataTier::StatementLine_Rec>> 
		get_statement_lines_for_account(const std::string& account, date::sys_days statement_date);

};
