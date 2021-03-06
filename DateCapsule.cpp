#include "stdafx.h"
#include "DateCapsule.h"
#include <regex>
#include <chrono>

#include "Util.h"

using namespace std;

// import Util;


std::string DateCapsule::extract(const std::string & stringIncludingDate) const
{
	smatch m;
	// skips non digits at front, expect 2017-01-12 afterwards
	regex reg{ "[^[:digit:]]*([[:digit:]]{4}-[[:digit:]]{1,2}-[[:digit:]]{1,2})" };

	bool ok = regex_search(stringIncludingDate, m, reg);

	if (!ok)
	{
		throw ios_base::failure("Date as string should have format like 2017-01-12");
	}

	string dateInString = m.str(1);
	return dateInString;
}

std::chrono::sys_days DateCapsule::asDate() const
{
	return Util::to_date(m_dateInString);
}
