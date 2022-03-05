#pragma once

//#include <date.h>

#ifndef DLLS_H
#define MFC_UTILITIES_EXPORTS
#include <Dlls.h>
#endif


inline namespace MFC_LOW_LEVEL
{

	class MFC_UTILITIES_API Data
	{
		const LPARAM	m_data;
	public:
		Data(const std::string& str)
			: m_data{ Util::stringAsData(str)}
		{}
		/*
		Data(const CString& str)
			: m_data{ Util::stringAsData(str) }
		{}
		*/


		Data(date::sys_days dp)
			: m_data{ Util::weekDayAsData(dp)}
		{}
		 
		operator LPARAM() const noexcept
		{
			return m_data;
		}
	};

}