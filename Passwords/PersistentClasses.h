#pragma once

#include <sqlite_orm/sqlite_orm.h>


import Util;
//#include "..\JDMFCLibrary/Util.h"


using namespace std::string_literals;

enum class AcknowledgementType
{
	None, Partial, Full
};

#define VERSION_2

struct Location
{
	int id;
	std::string name;
	std::string url;
	std::string email;

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + name + " - "s + url + " " + email;
		return str;
	}

	std::string getPassword(std::chrono::year_month_day ymd);

};


struct Password
{
	int id;
	std::string password;
	std::chrono::sys_days begining_date;
	int fkey_location;

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + password + " "s + Util::to_string(begining_date);
		return str;
	}
};


/////////////////////////////////////////////////////////////////
///
///

using namespace sqlite_orm;

using als_l = alias_l<Location>;
using als_p = alias_p<Password>;
