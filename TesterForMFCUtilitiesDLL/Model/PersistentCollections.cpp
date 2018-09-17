#include "stdafx.h"


#include <vector>
#include <memory>
#include "DataTier/DataTier.h"

#include "ORM.Definitions.h"
#include "PersistentCollections.hpp"

using namespace DataTier;
using namespace std;

std::vector<std::shared_ptr<StatementLine>> PersistentCollections::get_statement_lines_for(
	const std::string& account, date::sys_days statement_date)
{
	auto& storage = ORM::ORM_DataTier::get_storage();
	auto res = storage.get_all<StatementLine>(where(c(&StatementLine::m_account_fid) == account && c(StatementLine::m_statement_date) == statement_date));
	return vector<std::shared_ptr<StatementLine>>{};

}