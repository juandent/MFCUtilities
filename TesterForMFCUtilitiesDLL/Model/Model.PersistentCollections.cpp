#include "StdAfx.h"

#include <string>
#include <vector>
#include <memory>
#include "../DataTier/DataTier.h"
#include "../DataTier/DataTier.ORM.Definitions.h"

#include "Model.PersistentCollections.hpp"

//using namespace DataTier;

namespace Model
{
	using namespace std;
	using namespace sqlite_orm;

	std::vector<std::shared_ptr<DataTier::StatementLine>> PersistentCollections::get_statement_lines_for(
		const std::string& account, date::sys_days statement_date)
	{
		auto& storage = ORM::ORM_Central::refresh_and_get_schema();
		storage.get_no_throw<DataTier::StatementLine>(450);
		auto res = storage.get_all<DataTier::StatementLine>(where(c(&DataTier::StatementLine::m_account_fid) == "3777-XXXXXX-X6745"s && c(&DataTier::StatementLine::m_statement_date) == statement_date));


		return vector<std::shared_ptr<DataTier::StatementLine>>{};

	}
}
