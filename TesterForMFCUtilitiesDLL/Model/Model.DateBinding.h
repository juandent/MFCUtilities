#pragma once

#include <sqlite_orm/sqlite_orm.h>
#include <date/date.h>
#include <sstream>
#include <regex>

/**
 *  This is a enum we want to map to our sqlite db.
 *  First we have to decide what db type enum must have.
 *  Let's make it `TEXT`: Gender::Male will store as 'male' string
 *  and Gender::Female as 'female' string.
 */
enum class Gender {
	Male,
	Female,
};

//  also we need transform functions to make string from enum..
inline std::string SysDaysToString(date::sys_days pt) {
#if 0
	auto ret = date::format("%F", pt);
	return ret;
#else
	date::year_month_day ymd{ pt };
	std::ostringstream os;
	os << ymd.year();
	os << "-";
	os << static_cast<unsigned>(ymd.month());
	os << "-";
	os << ymd.day();
	return os.str();
#endif
}


constexpr auto null_sys_day = date::sys_days{ date::days{0} };


/**
 *  and enum from string. This function has nullable result cause
 *  string can be neither `male` nor `female`. Of course we
 *  won't allow this to happed but as developers we must have
 *  a scenario for this case.
 *  These functions are just helpers. They will be called from several places
 *  that's why I placed it separatedly. You can use any transformation type/form
 *  (for example BETTER_ENUM https://github.com/aantron/better-enums)
 */
inline date::sys_days SysDaysFromString(const std::string &s) {
#if 0
	std::stringstream ss{ s };
	date::sys_days tt;
	ss >> date::parse("%F", tt);
	if (!ss.fail())
	{
		return tt;
	}
	return null_sys_day;
#else
	using namespace std;

	smatch m;
	regex search_ymd{ "([[:d:]]{4})-([[:d:]]{1,2})-([[:d:]]{1,2})" };
	bool found_ymd = regex_search(s, m, search_ymd);
	if (found_ymd)
	{
		auto year = m[1].str();
		int yi = stoi(year);
		auto month = m[2].str();
		unsigned mi = stoi(month);
		auto day = m[3].str();
		unsigned di = stoi(day);

		date::year_month_day ymd{ date::year{yi}, date::month{mi}, date::day{di} };
		return ymd;
	}
	return null_sys_day;
#endif
	
}

/**
 *  This is where magic happens. To tell sqlite_orm how to act
 *  with Gender enum we have to create a few service classes
 *  specializations (traits) in sqlite_orm namespace.
 */
namespace sqlite_orm {

	/**
	 *  First of all is a type_printer template class.
	 *  It is responsible for sqlite type string representation.
	 *  We want Gender to be `TEXT` so let's just derive from
	 *  text_printer. Also there are other printers: real_printer and
	 *  integer_printer. We must use them if we want to map our type to `REAL` (double/float)
	 *  or `INTEGER` (int/long/short etc) respectively.
	 */
	template<>
	struct type_printer<date::sys_days> : public text_printer {};

	/**
	 *  This is a binder class. It is used to bind c++ values to sqlite queries.
	 *  Here we have to create gender string representation and bind it as string.
	 *  Any statement_binder specialization must have `int bind(sqlite3_stmt*, int, const T&)` function
	 *  which returns bind result. Also you can call any of `sqlite3_bind_*` functions directly.
	 *  More here https://www.sqlite.org/c3ref/bind_blob.html
	 */
	template<>
	struct statement_binder<date::sys_days> {

		int bind(sqlite3_stmt *stmt, int index, const date::sys_days &value) {
			return statement_binder<std::string>().bind(stmt, index, SysDaysToString(value));
			
		}
	};

	/**
	 *  field_printer is used in `dump` and `where` functions. Here we have to create
	 *  a string from mapped object.
	 */
	template<>
	struct field_printer<date::sys_days> {
		std::string operator()(const date::sys_days &t) const {
			return SysDaysToString(t);
		}
	};

	/**
	 *  This is a reverse operation: here we have to specify a way to transform string received from
	 *  database to our Gender object. Here we call `GenderFromString` and throw `std::runtime_error` if it returns
	 *  nullptr. Every `row_extractor` specialization must have `extract(const char*)` and `extract(sqlite3_stmt *stmt, int columnIndex)`
	 *  functions which return a mapped type value.
	 */
	template<>
	struct row_extractor<date::sys_days> {
		date::sys_days extract(const char *row_value) {
			auto sd = SysDaysFromString(row_value);
			if (sd != null_sys_day) {
				return sd;
			}
			else {
				throw std::runtime_error("incorrect date string (" + std::string(row_value) + ")");
			}
		}

		date::sys_days extract(sqlite3_stmt *stmt, int columnIndex) {
			auto str = sqlite3_column_text(stmt, columnIndex);
			return this->extract((const char*)str);
		}
	};
}

