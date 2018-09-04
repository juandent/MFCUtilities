#pragma once

//#include "date.h"

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

	date::sys_days asDate() const { return JD::to_date(m_dateInString); }
	std::string		asString() const { return m_dateInString;  }
};