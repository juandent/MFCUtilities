#pragma once

#include <sqlite_orm/sqlite_orm.h>


enum class AccountType
{
	CreditCard,
	BankAccount
};

inline std::shared_ptr<std::string> AccountTypeToString(AccountType type) {
	switch (type) {
	case AccountType::CreditCard : return std::make_shared<std::string>("CC");
	case AccountType::BankAccount :return std::make_shared<std::string>("BA");
	}
}

inline std::shared_ptr<AccountType> AccountTypeFromString(const std::string &s) {
	if (s == "CC") {
		return std::make_shared<AccountType>(AccountType::CreditCard);
	}
	else if (s == "BA") {
		return std::make_shared<AccountType>(AccountType::BankAccount);
	}
	else
		return nullptr;
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
	struct type_printer<AccountType> : public type_printer<std::string> {}; // public text_printer{};

	/**
	 *  This is a binder class. It is used to bind c++ values to sqlite queries.
	 *  Here we have to create gender string representation and bind it as string.
	 *  Any statement_binder specialization must have `int bind(sqlite3_stmt*, int, const T&)` function
	 *  which returns bind result. Also you can call any of `sqlite3_bind_*` functions directly.
	 *  More here https://www.sqlite.org/c3ref/bind_blob.html
	 */
	template<>
	struct statement_binder<AccountType> {

		int bind(sqlite3_stmt *stmt, int index, const AccountType &value) {
			if (auto str = AccountTypeToString(value)) {
				return statement_binder<std::string>().bind(stmt, index, *str);
			}
			else {
				return statement_binder<std::nullptr_t>().bind(stmt, index, nullptr);
			}

			//return statement_binder<std::wstring>().bind(stmt, index, AccountTypeToString(value));
			//  or return sqlite3_bind_text(stmt, index++, GenderToString(value).c_str(), -1, SQLITE_TRANSIENT);
		}
	};

	/**
	 *  field_printer is used in `dump` and `where` functions. Here we have to create
	 *  a string from mapped object.
	 */
	template<>
	struct field_printer<AccountType> {
		std::string operator()(const AccountType &t) const {
			if (auto res = AccountTypeToString(t)) {
				return *res;
			}
			else {
				return "?";
			}
		}
	};

	/**
	 *  This is a reverse operation: here we have to specify a way to transform string received from
	 *  database to our Gender object. Here we call `GenderFromString` and throw `std::runtime_error` if it returns
	 *  nullptr. Every `row_extractor` specialization must have `extract(const char*)` and `extract(sqlite3_stmt *stmt, int columnIndex)`
	 *  functions which return a mapped type value.
	 */
	template<>
	struct row_extractor<AccountType> {
		AccountType extract(const char *row_value) {
			if (auto AccountType = AccountTypeFromString(row_value)) {
				return *AccountType;
			}
			else {
				throw std::runtime_error("incorrect AccountType string (" + std::string(row_value) + ")");
			}
		}
		AccountType	extract(sqlite3_stmt *stmt, int columnIndex) {
			auto str = sqlite3_column_text(stmt, columnIndex);
			return this->extract((const char*)str);
		}
	};
}

