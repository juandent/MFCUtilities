#pragma once
// #include <date/date.h>
#include "Model.DataTier.h"

namespace Model
{

	class StatementBuilder
	{
	public:
		std::string								m_fileName;
		std::string								m_filePath;
		date::sys_days							m_statementDate;

		auto build() const
		{
			return std::make_shared<const Statement>(
				Statement{
					m_fileName,
					m_filePath,
					m_statementDate});
		}
	};

}