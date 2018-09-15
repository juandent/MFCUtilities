#pragma once
#include "DataTier.h"


class PersistentCollections
{
public:
	static std::vector<std::shared_ptr<DataTier::StatementLine>> 
		get_statement_lines_for(const std::string& account, date::sys_days statement_date);

};
