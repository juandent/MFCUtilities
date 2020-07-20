#pragma once

#undef min
#undef max

#include <sqlite_orm/sqlite_orm.h>
#include <catch2/catch.hpp>
#include <date/date.h>

#include "../FixedPoint/Money.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.DateBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Colones.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.CoinBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Dolares.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.AccountType.Binding.h"

#include "Nullable.h"


////////// persistence structs//
struct Statement;
struct StatementLine;
struct Concepto;
struct Account;
// struct MyAccount;
struct Pais;
struct Banco;
struct AccountOwner;
struct Transaccion;
struct Categoria;
////////////////////////////////


struct Statement
{
	int id_statement;
	date::sys_days date;
};

struct StatementLine
{
	int id_statement_line;
	date::sys_days date;
	std::string description;
	int fkey_concepto;
	std::optional<int> fkey_category;						// Categoria
	int fkey_statement;
};

struct Concepto
{
	int id_concepto;
	std::string name;				// TFT-SINPE A: 15103-02**-****-8467
	int fkey_account;				// { 15103-02**-****-8467, ...}
};

struct Account
{
	int id_account;
	std::string number;				// 15103-02**-****-8467
	int fkey_bank;					// { BAC San Jose, "Barrio Dent", { Costa Rica} }
	int fkey_account_owner;			// { Juan Dent Herrera, ... }
	std::string description;		// AMEX Cashback Premium
	bool is_tarjeta;				// true
};

#if 0
struct MyAccount
{
	int id_account;
	std::string number;				// 15103-02**-****-8467
	int fkey_bank;					// { BAC San Jose, "Barrio Dent", { Costa Rica} }
	int fkey_account_owner;			// { Juan Dent Herrera, ... }
	std::string description;		// AMEX Cashback Premium
	bool is_tarjeta;				// true
};
#endif

struct Pais
{
	int id_pais;
	std::string name;
};

struct Banco
{
	int id_bank;
	std::string nombre;
	std::string ubicacion;
	int fkey_pais;
};

struct AccountOwner
{
	int id_owner;
	std::string name;
};


struct Transaccion
{
	int id_transaccion;
	double amount_colones;
	double amount_dolares;
	int fkey_account_own;				// Account
	std::optional<int> fkey_account_other;			// Account optional
	int fkey_statementline;					// StatementLine

	std::shared_ptr<StatementLine> getStatementLine() const;
	std::shared_ptr<Account> getAccountOrigin() const;
	std::shared_ptr<Account> getAccountDestination() const;

};

struct Categoria
{
	int id_categoria;
	std::string name;
	bool is_expense_or_income;
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
			make_table( "Statement",
				make_column("id_statement", &Statement::id_statement, autoincrement(), primary_key()),
				make_column("date", &Statement::date)),
			make_table("Categoria",
				make_column("id_category", &Categoria::id_categoria, autoincrement(), primary_key()),
				make_column("name", &Categoria::name, collate_nocase()),
				make_column("is_expense_or_income", &Categoria::is_expense_or_income)),
			make_table("StatementLine",
				make_column("id_statement_line", &StatementLine::id_statement_line, autoincrement(), primary_key()),
				make_column("date", &StatementLine::date),
				make_column("description", &StatementLine::description, collate_nocase()),
				make_column("fkey_concepto", &StatementLine::fkey_concepto),
				make_column("fkey_categoria", &StatementLine::fkey_category),
				make_column("fkey_statement", &StatementLine::fkey_statement),
				foreign_key(&StatementLine::fkey_category).references(&Categoria::id_categoria),
				foreign_key(&StatementLine::fkey_concepto).references(&Concepto::id_concepto),
				foreign_key(&StatementLine::fkey_statement).references(&Statement::id_statement)),
				make_table("Concepto",
					make_column("id_concepto", &Concepto::id_concepto, autoincrement(), primary_key()),
					make_column("name", &Concepto::name, collate_nocase()),
					make_column("fkey_account", &Concepto::fkey_account),
					foreign_key(&Concepto::fkey_account).references(&Account::id_account)),
				make_table("Account",
					make_column("id_account", &Account::id_account, autoincrement(), primary_key()),
					make_column("number", &Account::number, collate_nocase()),
					make_column("fkey_bank", &Account::fkey_bank),
					make_column("fkey_account_owner", &Account::fkey_account_owner),
					make_column("description", &Account::description, collate_nocase()),
					make_column("is_tarjeta", &Account::is_tarjeta),
					foreign_key(&Account::fkey_account_owner).references(&AccountOwner::id_owner),
					foreign_key(&Account::fkey_bank).references(&Banco::id_bank)),
			// make_table("MyAccount",
			// 	make_column("id_account", &MyAccount::id_account, autoincrement(), primary_key()),
			// 	make_column("number", &MyAccount::number, collate_nocase()),
			// 	make_column("fkey_bank", &MyAccount::fkey_bank),
			// 	make_column("fkey_account_owner", &MyAccount::fkey_account_owner),
			// 	make_column("description", &MyAccount::description, collate_nocase()),
			// 	make_column("is_tarjeta", &MyAccount::is_tarjeta),
			// 	foreign_key(&MyAccount::fkey_account_owner).references(&AccountOwner::id_owner),
			// 	foreign_key(&MyAccount::fkey_bank).references(&Banco::id_bank)),
			make_table("Banco",
				make_column("id_bank", &Banco::id_bank, autoincrement(), primary_key()),
				make_column("nombre", &Banco::nombre, collate_nocase()),
				make_column("ubicacion", &Banco::ubicacion, collate_nocase()),
				make_column("fkey_Pais", &Banco::fkey_pais),
				foreign_key(&Banco::fkey_pais).references(&Pais::id_pais)),
			make_table("AccountOwner",
				make_column("id_owner", &AccountOwner::id_owner, autoincrement(), primary_key()),
				make_column("name", &AccountOwner::name, collate_nocase())),
			make_table("Transaccion",
				make_column("id_transaccion", &Transaccion::id_transaccion, autoincrement(), primary_key()),
				make_column("colones", &Transaccion::amount_colones),
				make_column("dolares", &Transaccion::amount_dolares),
				make_column("fkey_account_own", &Transaccion::fkey_account_own),
				make_column("fkey_account_other", &Transaccion::fkey_account_other),
				make_column("fkey_statementline", &Transaccion::fkey_statementline),
				foreign_key(&Transaccion::fkey_account_own).references(&Account::id_account),
				foreign_key(&Transaccion::fkey_account_other).references(&Account::id_account),
				foreign_key(&Transaccion::fkey_statementline).references(&StatementLine::id_statement_line)),
			make_table("Pais",
				make_column("id_pais", &Pais::id_pais, autoincrement(), primary_key()),
				make_column("name", &Pais::name, collate_nocase()))
			);

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
