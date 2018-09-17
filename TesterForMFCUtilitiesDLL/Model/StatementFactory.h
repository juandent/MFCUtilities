#pragma once

#include <memory>
#include <string>
#include "../DataTier/DataTier.h"

class CSVFile;


class StatementFactory
{
	static AccountType selectType(const CSVFile& csv);
public:
	static std::shared_ptr<DataTier::Statement> Create(const std::string& rawFileName, date::sys_days dp);
};

