#pragma once

#include <memory>
#include <string>
#include "Model.DataTier.h"
#include "Model.AccountType.Binding.h"

namespace csv
{
	class CSV_File;
}

namespace Model
{

	class StatementFactory
	{
		static AccountType selectType(const csv::CSV_File& csv);
	public:
		static std::shared_ptr<Statement> Create(const std::string& rawFileName, date::sys_days dp);
	};

}
