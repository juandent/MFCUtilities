#pragma once

#include <string>
// #include <date/date.h>
// #include "Utilities.h"
#include <chrono>

#ifndef DLLS_H
#define MFC_UTILITIES_EXPORTS
#include <Dlls.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A date capsule. Encapsulates a date given as a string and 
/// 			allows the date to be extracted </summary>
///
/// <remarks>	Juan, 5/4/2017. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class MFC_UTILITIES_API DateCapsule
{
	const std::string m_dateInString;
	std::string extract(const std::string& stringIncludingDate) const;
public:
	DateCapsule(const std::string& stringIncludingDate)
		: m_dateInString{ extract(stringIncludingDate) }
	{}

	std::chrono::sys_days asDate() const;
	std::string		asString() const { return m_dateInString;  }
};