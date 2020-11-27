#pragma once

#undef min
#undef max

#include <sqlite_orm/sqlite_orm.h>
// #include <catch2/catch.hpp>
#include <date/date.h>

#include "../FixedPoint/Money.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.DateBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Colones.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.CoinBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Dolares.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.AccountType.Binding.h"

// #include "Nullable.h"


////////// persistence structs//
struct Statement;
struct Concepto;
struct Account;
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

	Pais getPais() const;
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
	
	date::sys_days line_date;
	std::string descripcion;
	int fkey_category;
	int fkey_concepto;
	int fkey_statement;
	int row;					 // fkey_statement + row is unique

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
	// constexpr const char* db_name{ "ExpenseManagerSQLite.sqlite" };

	constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\ExpenseManagerMFC\\ExpenseManagerSQLite\\ExpenseManagerSQLite.sqlite" };

	
	static auto storage =
		make_storage(db_name,
			make_table( "Statement",
				make_column("id_statement", &Statement::id_statement, autoincrement(), primary_key()),
				make_column("date", &Statement::date)),
			make_table("Categoria",
				make_column("id_category", &Categoria::id_categoria, autoincrement(), primary_key()),
				make_column("name", &Categoria::name, collate_nocase()),
				make_column("is_expense_or_income", &Categoria::is_expense_or_income)),
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
				make_column("line_date", &Transaccion::line_date),
				make_column( "descripcion", &Transaccion::descripcion),
				make_column("fkey_category", &Transaccion::fkey_category),
				make_column( "concepto", &Transaccion::fkey_concepto),
				make_column( "fkey_statement", &Transaccion::fkey_statement),
				make_column("row", &Transaccion::row),
				foreign_key(&Transaccion::fkey_account_own).references(&Account::id_account),
				foreign_key(&Transaccion::fkey_account_other).references(&Account::id_account),
				foreign_key(&Transaccion::fkey_category).references(&Categoria::id_categoria),
				foreign_key(&Transaccion::fkey_concepto).references(&Concepto::id_concepto),
				foreign_key(&Transaccion::fkey_statement).references(&Statement::id_statement)),
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


enum class Operation { doUpdate = 1, doInsert = 2 };


template<typename Table>
std::optional<Table> getCurrent(CEdit& editCtrl)
{
	using namespace sqlite_orm;

	CString rId;
	editCtrl.GetWindowTextW(rId);
	if( rId.IsEmpty())
	{
		return std::nullopt;
	}
	auto id_str = JD::from_cstring(rId);
	auto id = std::stoi(id_str);
	auto record = Storage::getStorage().get_optional < Table>(id);
	///////////	
	return record;
}

template<typename Table>
Operation whatOperation(CEdit& editCtrl)
{
	auto record = getCurrent<Table>(editCtrl);
	return record ? Operation::doUpdate : Operation::doInsert;
}

template<typename Table>
void setIdFromRecord(CEdit& editCtrl, int pk)
{
	editCtrl.SetWindowTextW(JD::to_cstring(pk));
}

template<typename Table, int Table::* key>
struct IdManager
{
	CEdit& editCtrl;
	std::optional<Table>& record;
	IdManager(CEdit& editCtrl, std::optional<Table>& record)
		: editCtrl(editCtrl), record{ record }
	{}
	void SetId()
	{
		editCtrl.SetWindowTextW(JD::to_cstring(record->*key));
	}
	std::optional<Table> GetIdToCurrent()
	{
		record = getCurrent<Table>(editCtrl);
	}
};

