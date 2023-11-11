#pragma once

// #include <FixedPoint/Money.h>
// #include "Utilities.h"

import util;

using namespace util;

#include <regex>
#include <array>

#include <sqlite_orm/sqlite_orm.h>

////// Colones

inline std::string ColonesToString(util::Colones coin) {
	std::string temp = static_cast<std::string>(coin);
	return temp;
}

inline util::Colones ColonesFromString(const std::string &s) {
	using namespace std;

	string chars_to_remove = "[$]|[¢]|,|";
	regex remove{chars_to_remove};

	array<char, 40> buffer;
	auto last = std::regex_replace(buffer.begin(), s.begin(), s.end(), remove, "");

	auto dist = distance(buffer.begin(), last);

	buffer[dist] = 0;
	string res{ buffer.data()};
	auto ret = stold(res);
	return ret;
}


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
	struct type_printer<util::Colones> : public text_printer {}; // type_printer<std::wstring> {};  // text_printer{};

	/**
	 *  This is a binder class. It is used to bind c++ values to sqlite queries.
	 *  Here we have to create gender string representation and bind it as string.
	 *  Any statement_binder specialization must have `int bind(sqlite3_stmt*, int, const T&)` function
	 *  which returns bind result. Also you can call any of `sqlite3_bind_*` functions directly.
	 *  More here https://www.sqlite.org/c3ref/bind_blob.html
	 */
	template<>
	struct statement_binder<util::Colones> {

		int bind(sqlite3_stmt *stmt, int index, const util::Colones &value) {
			return statement_binder<std::string>().bind(stmt, index, ColonesToString(value));
			//  or return sqlite3_bind_text(stmt, index++, GenderToString(value).c_str(), -1, SQLITE_TRANSIENT);
		}
	};

	/**
	 *  field_printer is used in `dump` and `where` functions. Here we have to create
	 *  a string from mapped object.
	 */
	template<>
	struct field_printer<util::Colones> {
		std::string operator()(const util::Colones &t) const {
			return ColonesToString(t);
		}
	};

	/**
	 *  This is a reverse operation: here we have to specify a way to transform string received from
	 *  database to our Gender object. Here we call `GenderFromString` and throw `std::runtime_error` if it returns
	 *  nullptr. Every `row_extractor` specialization must have `extract(const char*)` and `extract(sqlite3_stmt *stmt, int columnIndex)`
	 *  functions which return a mapped type value.
	 */
//#define USING_INHERITANCE
#ifdef USING_INHERITANCE
	template<>
	struct row_extractor<Colones> : public row_extractor<std::wstring> {
		Colones extract(sqlite3_stmt *stmt, int columnIndex) {
			auto str = sqlite3_column_text(stmt, columnIndex);
			auto ws = row_extractor<std::wstring>::extract( (const char*)str);
			auto s = util::to_string(ws);
			return ColonesFromString(s);
		}
	};
#else
#if 1	// use std::string
	template<>
	struct row_extractor<util::Colones> {
		util::Colones extract(const char *row_value) {
			return ColonesFromString(row_value);
		}

		util::Colones extract(sqlite3_stmt *stmt, int columnIndex) {
			auto str = sqlite3_column_text(stmt, columnIndex);
			return this->extract((const char*)str);
		}
};

#else
	template<>
	struct row_extractor<Colones> {
		Colones extract(sqlite3_stmt *stmt, int columnIndex) {
			auto str = sqlite3_column_text(stmt, columnIndex);
			auto ws = row_extractor<std::wstring>().extract((const char*)str);
			return ColonesFromString(ws);
		}
	};
#endif
#endif
}

