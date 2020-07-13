#pragma once

#undef min
#undef max

#include <sqlite_orm/sqlite_orm.h>
#include <catch2/catch.hpp>
#include <date/date.h>

#include "../FixedPoint/Money.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.DateBinding.h"
//#include "..\TesterForMFCUtilitiesDLL/Model/Model.Colones.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.CoinBinding.h"
//#include "..\TesterForMFCUtilitiesDLL/Model/Model.Dolares.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.AccountType.Binding.h"


struct StatementLine
{
	int id_statement_line;
	date::sys_days date;
	std::string description;
	int fkey_concepto;
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
struct Concepto_Account
{
	int fkey_concepto;
	int fkey_account;	
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
	int fkey_account_origin;				// Account
	int fkey_account_destination;			// Account
	int fkey_statementline;					// StatementLine

	std::shared_ptr<StatementLine> getStatementLine() const;
	std::shared_ptr<Account> getAccountOrigin() const;
	std::shared_ptr<Account> getAccountDestination() const;

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
				foreign_key(&StatementLine::fkey_concepto).references(&Concepto::id_concepto)),
				make_table("Concepto",
					make_column("id_concepto", &Concepto::id_concepto, autoincrement(), primary_key()),
					make_column("name", &Concepto::name),
					make_column("fkey_account", &Concepto::fkey_account),
					foreign_key(&Concepto::fkey_account).references(&Account::id_account)),
				make_table("Account",
					make_column("id_account", &Account::id_account, autoincrement(), primary_key()),
					make_column("number", &Account::number),
					make_column("fkey_bank", &Account::fkey_bank),
					make_column("fkey_account_owner", &Account::fkey_account_owner),
					make_column("description", &Account::description),
					make_column("is_tarjeta", &Account::is_tarjeta),
					foreign_key(&Account::fkey_account_owner).references(&AccountOwner::id_owner),
					foreign_key(&Account::fkey_bank).references(&Banco::id_bank)),
				make_table("Banco",
					make_column("id_bank", &Banco::id_bank, autoincrement(), primary_key()),
					make_column("nombre", &Banco::nombre),
					make_column("ubicacion", &Banco::ubicacion),
					make_column("fkey_Pais", &Banco::fkey_pais),
					foreign_key(&Banco::fkey_pais).references(&Pais::id_pais)),
			make_table("AccountOwner",
				make_column("id_owner", &AccountOwner::id_owner, autoincrement(), primary_key()),
				make_column("name", &AccountOwner::name)),
			make_table("Transaccion",
				make_column("id_transaccion", &Transaccion::id_transaccion, autoincrement(), primary_key()),
				make_column("colones", &Transaccion::amount_colones),
				make_column("dolares", &Transaccion::amount_dolares),
				make_column("fkey_account_origin", &Transaccion::fkey_account_origin),
				make_column("fkey_account_destination", &Transaccion::fkey_account_destination),
				make_column("fkey_statementline", &Transaccion::fkey_statementline),
				foreign_key(&Transaccion::fkey_account_origin).references(&Account::id_account),
				foreign_key(&Transaccion::fkey_account_destination).references(&Account::id_account),
				foreign_key(&Transaccion::fkey_statementline).references(&Transaccion::id_transaccion)),
			make_table("Pais",
				make_column("id_pais", &Pais::id_pais, autoincrement(), primary_key()),
				make_column("name", &Pais::name))
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
