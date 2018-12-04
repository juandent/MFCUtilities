#pragma once
//#include "../Model/Model.DataTier.h"
#include <vector>
#include <memory>
#include "../ORM/ORM.Repository.h"


class StatementLineRepository : public ORM::Repository<Model::StatementLine>
{
public:

	template<typename C = std::vector<Model::StatementLine>>
	C getStatementLinesFor(const std::string& account, date::sys_days statement_date)
	{
		using namespace sqlite_orm;
		using namespace Model;

		auto& whereClause = c(&Element::m_account_fid) == account && c(&Element::m_statement_date) == statement_date;

		return Repository::Find<C>(whereClause);
	}

};
