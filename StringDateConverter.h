#pragma once

#include <string>
#include <regex>
#include <date.h>
#include <array>
#include <utility>
#include <map>

#ifndef DLLS_H
#define MFC_UTILITIES_EXPORTS
#include <Dlls.h>
#endif

// date as string comes in 2 flavors:

//	when it is a complete date (includes d, m and year)
//	when it is not (only includes d and m)

namespace DateAsString
{
	class MFC_UTILITIES_API MonthNames
	{
		static std::array<std::string, 12> s_monthNames;
	public:
		MonthNames() = delete;	// prohibit instance creation!
		static std::pair<bool,unsigned> getMonthNumber(const std::string& name);
	};

	class MFC_UTILITIES_API IDateExtractor
	{
	protected:
		IDateExtractor() = default;
	public:
		virtual std::pair<bool, date::sys_days> convert(const std::string& asText) const = 0;
		virtual ~IDateExtractor() = default;
	};

	class MFC_UTILITIES_API CompleteDateExtractor : public IDateExtractor
	{
		const std::regex m_regularExpression;
		const int  m_dayIndex, m_monthIndex, m_yearIndex;
	public:
		CompleteDateExtractor(const std::string& expression, int dayIndex, int monthIndex, int yearIndex)
			: m_regularExpression{ std::regex{ expression } }, m_dayIndex{ dayIndex }, m_monthIndex{ monthIndex }, m_yearIndex{ yearIndex }
		{}
		virtual std::pair<bool, date::sys_days> convert(const std::string& asText) const override;
	};

	inline int yearFromLineMonthAndStatement(unsigned lineMonth, unsigned statementMonth, unsigned statementYear)
	{
		if (lineMonth == statementMonth)
		{
			return statementYear;
		}
		else if (statementMonth == 1)
		{
			return statementYear - 1;
		}
		else
		{
			return statementYear;
		}
	}

	// text lacks year!! must calculate from Statement date!
	class MFC_UTILITIES_API IncompleteDateExtractor : public IDateExtractor
	{
		const std::regex m_regularExpression;
		const int  m_dayIndex, m_monthIndex;
		const unsigned	m_statementMonth;
		const int m_statementYear;


	public:
		IncompleteDateExtractor(const std::string& expression, int dayIndex, int monthIndex, unsigned statementMonth, int statementYear)
			: m_regularExpression{ std::regex{ expression } }, m_dayIndex{ dayIndex }, m_monthIndex{ monthIndex }, m_statementMonth{statementMonth}, m_statementYear{statementYear}
		{}
		virtual std::pair<bool, date::sys_days> convert(const std::string& asText) const override;
	};

	bool MFC_UTILITIES_API isDateComplete(const std::string& asText);


	class MFC_UTILITIES_API StringDateConverter
	{
		const std::string	m_asText;
		const unsigned		m_statementMonth;
		const int			m_statementYear;

		static std::map<std::string, std::array<int,3>>		s_expressions_for_complete_dates;
		static std::map<std::string, std::array<int, 2>>	s_expressions_for_incomplete_dates;
	public:
		StringDateConverter( const std::string& asText, unsigned statementMonth, int statementYear)
			: m_asText{asText}, m_statementMonth{statementMonth}, m_statementYear{statementYear}
		{}
		std::pair<bool, date::sys_days> convert() const;
	};
}
