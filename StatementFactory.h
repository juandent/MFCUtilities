#pragma once

#include <memory>
#include <string>
#include "CSVFile.h"

class Statement;

enum class StatementTypes { Unkwnon, BAC_BankAccount, BAC_CreditCard };


class StatementFactory
{
	static StatementTypes selectType(const CSVFile& csv);
public:
	static std::shared_ptr<Statement> Create(const std::string& rawFileName, date::sys_days dp);
};

