#pragma once
#include <date/date.h>
#include "../DataTier/DataTier.h"


class StatementBuilder
{
public:
	std::string				m_fileName;
	std::string				m_filePath;
	date::sys_days			m_statementDate;
	std::string				m_account_fid;

	auto build() const
	{
		return std::make_shared<const DataTier::Statement>(
			DataTier::Statement{
				m_fileName,
				m_filePath,
				m_statementDate,
				m_account_fid });
	}
};
