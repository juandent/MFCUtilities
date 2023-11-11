#include "stdafx.h"
#include "Money.h"
// #include "Utilities.h"

import Util;


Colones::operator std::string()
{
	return Util::Local::to_string(*this, m_width);
}

Colones::operator std::wstring()
{
	return Util::Local::to_wstring(*this, m_width);
}



Dolares::operator std::string()
{
	return Util::to_string(*this, m_width);
}

Dolares::operator std::wstring()
{
	return Util::to_wstring(*this, m_width);
}

#if 0
namespace Util
{

	std::string to_string(Colones money, int width)
	{
		// auto temp = moneyHelper->putColones(money, width);
		return "f";
	}

	std::wstring to_wstring(Colones money, int width)
	{
		// auto temp = to_string(money, width);
		return L"g";// to_wstring(temp);
	}

}
#endif
