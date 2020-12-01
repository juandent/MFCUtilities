#pragma once

#pragma once

#include <FixedPoint/arithmetic_types.h>

// #include "Utilities.h"
#include <regex>
#include <array>

#include <sqlite_orm/sqlite_orm.h>

import Util;

////// Dolares

inline std::string DolaresToString(Dolares coin) {
	std::string temp = coin;
	return temp;
}

inline Dolares DolaresFromString(const std::string &s) {
	using namespace std;

	string chars_to_remove = "[$]|[¢]|,|";
	regex remove{ chars_to_remove };

	array<char, 40> buffer;
	auto last = std::regex_replace(buffer.begin(), s.begin(), s.end(), remove, "");

	auto dist = distance(buffer.begin(), last);

	buffer[dist] = 0;
	string res{ buffer.data() };
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
	struct type_printer<Dolares> : public text_printer{};

	/**
	 *  This is a binder class. It is used to bind c++ values to sqlite queries.
	 *  Here we have to create gender string representation and bind it as string.
	 *  Any statement_binder specialization must have `int bind(sqlite3_stmt*, int, const T&)` function
	 *  which returns bind result. Also you can call any of `sqlite3_bind_*` functions directly.
	 *  More here https://www.sqlite.org/c3ref/bind_blob.html
	 */
	template<>
	struct statement_binder<Dolares> {

		int bind(sqlite3_stmt *stmt, int index, const Dolares &value) {
			return statement_binder<std::string>().bind(stmt, index, DolaresToString(value));
			//  or return sqlite3_bind_text(stmt, index++, GenderToString(value).c_str(), -1, SQLITE_TRANSIENT);
		}
	};

	/**
	 *  field_printer is used in `dump` and `where` functions. Here we have to create
	 *  a string from mapped object.
	 */
	template<>
	struct field_printer<Dolares> {
		std::string operator()(const Dolares &t) const {
			return DolaresToString(t);
		}
	};

	/**
	 *  This is a reverse operation: here we have to specify a way to transform string received from
	 *  database to our Gender object. Here we call `GenderFromString` and throw `std::runtime_error` if it returns
	 *  nullptr. Every `row_extractor` specialization must have `extract(const char*)` and `extract(sqlite3_stmt *stmt, int columnIndex)`
	 *  functions which return a mapped type value.
	 */
	template<>
	struct row_extractor<Dolares> {
		Dolares extract(const char *row_value) {
			return DolaresFromString(row_value);
		}

		Dolares extract(sqlite3_stmt *stmt, int columnIndex) {
			auto str = sqlite3_column_text(stmt, columnIndex);
			return this->extract((const char*)str);
		}
	};
}

