#pragma once

#include <sqlite_orm/sqlite_orm.h>


enum class Coin
{
	Colon = '¢',
	Dolar = '$',
	Both ,
	Unknown
};

inline std::wstring CoinToString(Coin coin) {
	switch (coin) {
	case Coin::Colon:return L"¢";
	case Coin::Dolar:return L"$";
	case Coin::Both: return L"¢$";
	}
}

#if 0
inline std::shared_ptr<Coin> CoinFromString(const std::wstring &s) {
	if (s == L"¢") {
		return std::make_shared<Coin>(Coin::Colon);
	}
	else if (s == L"$") {
		return std::make_shared<Coin>(Coin::Dolar);
	}
	else if (s == L"¢$")
	{
		return std::make_shared<Coin>(Coin::Both);
	}
	else
		return nullptr;
}
#else
inline Coin CoinFromString(const std::wstring &s) {
	if (s == L"¢") {
		return Coin::Colon;
	}
	else if (s == L"$") {
		return Coin::Dolar;
	}
	else if (s == L"¢$")
	{
		return Coin::Both;
	}
	else
		return Coin::Unknown;
}

#endif

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
	struct type_printer<Coin> : public type_printer<std::wstring> {}; // public text_printer{};

	/**
	 *  This is a binder class. It is used to bind c++ values to sqlite queries.
	 *  Here we have to create gender string representation and bind it as string.
	 *  Any statement_binder specialization must have `int bind(sqlite3_stmt*, int, const T&)` function
	 *  which returns bind result. Also you can call any of `sqlite3_bind_*` functions directly.
	 *  More here https://www.sqlite.org/c3ref/bind_blob.html
	 */
	template<>
	struct statement_binder<Coin> {

		int bind(sqlite3_stmt *stmt, int index, const Coin &value) {
			return statement_binder<std::wstring>().bind(stmt, index, CoinToString(value));
			//  or return sqlite3_bind_text(stmt, index++, GenderToString(value).c_str(), -1, SQLITE_TRANSIENT);
		}
	};

	/**
	 *  field_printer is used in `dump` and `where` functions. Here we have to create
	 *  a string from mapped object.
	 */
	template<>
	struct field_printer<Coin> {
		std::wstring operator()(const Coin &t) const {
			return CoinToString(t);
		}
	};

	/**
	 *  This is a reverse operation: here we have to specify a way to transform string received from
	 *  database to our Gender object. Here we call `GenderFromString` and throw `std::runtime_error` if it returns
	 *  nullptr. Every `row_extractor` specialization must have `extract(const char*)` and `extract(sqlite3_stmt *stmt, int columnIndex)`
	 *  functions which return a mapped type value.
	 */
	template<>
	struct row_extractor<Coin> {
#if 0
		Coin extract(const char *row_value) {
			if (auto coin = CoinFromString(row_value)) {
				return *coin;
			}
			else {
				throw std::runtime_error("incorrect coin string (" + std::string(row_value) + ")");
			}
		}
#endif
		Coin	extract(sqlite3_stmt *stmt, int columnIndex) {
			auto str = sqlite3_column_text(stmt, columnIndex);
			auto ws = row_extractor<std::wstring>().extract((const char*)str);
			return CoinFromString(ws);
			//return this->extract((const char*)str);
		}
	};
}

