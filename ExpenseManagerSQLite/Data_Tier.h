#pragma once

#undef min
#undef max

#include <sqlite_orm/sqlite_orm.h>
#include <catch2/catch.hpp>
#include <date/date.h>

#include "../FixedPoint/Money.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.DateBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Colones.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.DateBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.CoinBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Dolares.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.AccountType.Binding.h"


namespace sqlite_orm {
	template<>
	struct type_printer<long double, void> : public real_printer {};
}

struct StatementLine
{
	int id_statement_line;
	date::sys_days date;
	std::string description;
	int fkey_concepto;
	long double amount_colones;
	long double amount_dolares;
	int fkey_account;
};

struct Concepto
{
	int id_concepto;
	std::string name;
};

struct Account
{
	int id_account;
	std::string number;
	int fkey_bank;
	int fkey_account_owner;
	std::string description;
};

struct Concepto_Account
{
	int fkey_concepto;
	int fkey_account;	
};

struct Banco
{
	int id_bank;
	std::string nombre;
	std::string ciudad;
	std::string pais;
};

struct AccountOwner
{
	int id_owner;
	std::string name;
};



class Storage_Impl
{
private:
	Storage_Impl() = delete;	// prohibit instantiation
	static auto& get_storage();

	friend class Storage;

};

inline 	auto& Storage_Impl::get_storage()
{
	using namespace sqlite_orm;
	using namespace date;
	using namespace std;

	static int flag = 0;
	constexpr const char* db_name{ "ExpenseManagerSQLite.sqlite" };

	static auto storage =
		make_storage(db_name,
			make_table("StatementLine",
				make_column("id_statement_line", &StatementLine::id_statement_line, autoincrement(), primary_key()),
				make_column("date", &StatementLine::date),
				make_column("description", &StatementLine::description),
				make_column("fkey_concepto", &StatementLine::fkey_concepto),
				make_column("amount_colones", &StatementLine::amount_colones),
				make_column("amount_dolares", &StatementLine::amount_dolares),
				make_column("fkey_account", &StatementLine::fkey_account)),
			make_table("Concepto",
				make_column("id_concepto", &Concepto::id_concepto, autoincrement(), primary_key()),
				make_column("name", &Concepto::name)),
			make_table("Account",
				make_column("id_account", &Account::id_account, autoincrement(), primary_key()),
				make_column("number", &Account::number),
				make_column("fkey_bank", &Account::fkey_bank),
				make_column("fkey_account_owner", &Account::fkey_account_owner),
				make_column("description", &Account::description),
				foreign_key(&Account::fkey_account_owner).references(&AccountOwner::id_owner),
				foreign_key(&Account::fkey_bank).references(&Banco::id_bank)),
			make_table("Banco",
				make_column("id_bank", &Banco::id_bank, autoincrement(), primary_key()),
				make_column("nombre", &Banco::nombre),
				make_column("ciudad", &Banco::ciudad),
				make_column("pais", &Banco::pais)),
			make_table("AccountOwner",
				make_column("id_owner", &AccountOwner::id_owner, autoincrement(), primary_key()),
				make_column("name", &AccountOwner::name)),
			make_table("Concepto_Account",
				make_column("fkey_account", &Concepto_Account::fkey_account),
				make_column("fkey_concept", &Concepto_Account::fkey_concepto),
				foreign_key(&Concepto_Account::fkey_account).references(&Account::id_account),
				foreign_key(&Concepto_Account::fkey_concepto).references(&Concepto::id_concepto)));

	if (flag == 0)
	{
		flag = 1;
		storage.sync_schema(false);
	}

	return storage;
}

class Storage
{
public:
	using Storage_t = decltype(Storage_Impl::get_storage());

	Storage() = delete;
	static void initialize();
	static Storage_t& getStorage() { return Storage_Impl::get_storage(); }
	static void fill_db_with_test_data();
	static void empty_database();
};

//inline Storage::Storage_t& storage = Storage::getStorage();
