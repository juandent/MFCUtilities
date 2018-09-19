#pragma once

#include <memory>
#include <string>
#include "Model.DataTier.h"
#include "Model.AccountType.Binding.h"

class CSVFile;

namespace Model
{

	class StatementFactory
	{
		static AccountType selectType(const CSVFile& csv);
	public:
		static std::shared_ptr<Statement> Create(const std::string& rawFileName, date::sys_days dp);
	};

}
