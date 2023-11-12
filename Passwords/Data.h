#pragma once
#undef min
#undef max

#include <sqlite_orm/sqlite_orm.h>
// #include <catch2/catch.hpp>
#include <chrono>
// #include <date/date.h>


import Util;
import fixed_point;

//#include "..\JDMFCLibrary/Util.h"

// #include "../FixedPoint/Money.h"
#if 1
// #include "..\TesterForMFCUtilitiesDLL/Model/Model.DateBinding.h"
#include "..\ORM_Extensions/DateBinding.h"
#else
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Colones.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.CoinBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Dolares.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.AccountType.Binding.h"
#endif

#include "PersistentClasses.h"

import one_drive;

class Storage_Impl
{
private:
	Storage_Impl() = delete;	// prohibit instantiation
	static auto& get_storage();
	static auto& get_old_storage();
	static auto& get_new_storage();
	static void copy_old_to_new();

	friend class Storage;

	static inline std::string db_name = one_drive_path("\\Tokens\\Tokens.sqlite");
	// static constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\Tokens\\Tokens.sqlite" };
};


inline 	auto& Storage_Impl::get_storage()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;

	static int flag = 0;

	int id;
	std::string password;
	std::chrono::sys_days begining_date;
	int fkey_location;


	static auto storage =
		make_storage(db_name,
			make_table("Location",
				make_column("id_location", &Location::id, primary_key().autoincrement()),
				make_column("name", &Location::name),
				make_column("url", &Location::url),
				make_column("email", &Location::email)),
			make_table("Password",
				make_column("id_password", &Password::id, primary_key().autoincrement()),
				make_column("password", &Password::password),
				make_column("begin_date", &Password::begining_date),
				make_column("fkey_location", &Password::fkey_location ),
				foreign_key(&Password::fkey_location).references(&Location::id)));


	if (flag == 0)
	{
		flag = 1;
		storage.sync_schema(true);
	}

	return storage;
}


inline auto& Storage_Impl::get_old_storage()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;

	static int flag = 0;

	// this file will start full
	constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\Tokens\\old.sqlite" };

	constexpr int version = 2;

	static auto storage =
		make_storage(db_name,
			make_table("Location",
				make_column("id_location", &Location::id, primary_key().autoincrement()),
				make_column("name", &Location::name),
				make_column("url", &Location::url),
				make_column("email", &Location::email)),
				make_table("Password",
					make_column("id_password", &Password::id, primary_key().autoincrement()),
					make_column("password", &Password::password),
					make_column("begin_date", &Password::begining_date),
					make_column("fkey_location", &Password::fkey_location),
					foreign_key(&Password::fkey_location).references(&Location::id)));


	if (flag == 0)
	{
		flag = 1;
		storage.sync_schema(false);
	}

	return storage;
}

inline auto& Storage_Impl::get_new_storage()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;

	static int flag = 0;
	// this file will start empty
	constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\Tokens\\New.sqlite" };

	constexpr int version = 3;

	static auto storage =
		make_storage(db_name,
			make_table("Location",
				make_column("id_location", &Location::id, primary_key().autoincrement()),
				make_column("name", &Location::name),
				make_column("url", &Location::url),
				make_column("email", &Location::email)),
				make_table("Password",
					make_column("id_password", &Password::id, primary_key().autoincrement()),
					make_column("password", &Password::password),
					make_column("begin_date", &Password::begining_date),
					make_column("fkey_location", &Password::fkey_location),
					foreign_key(&Password::fkey_location).references(&Location::id)));

	if (flag == 0)
	{
		flag = 1;
		storage.sync_schema(true);
	}

	return storage;

}

class Storage
{
public:
	using Storage_t = decltype(Storage_Impl::get_storage());

	Storage();
	static void initialize();
	static Storage_t& getStorage() { return Storage_Impl::get_storage(); }
	static void fill_db_with_test_data();
	static void empty_database();
	static void upgrade_database();
	static void backup_db();
};

//inline Storage::Storage_t& storage = Storage::getStorage();

#include "..\ORM_Extensions/ORM_Utilities.h"

#if 0

enum class Operation { doUpdate = 1, doInsert = 2 };


template<typename Table>
std::optional<Table> getCurrent(CEdit& editCtrl)
{
	using namespace sqlite_orm;

	CString rId;
	editCtrl.GetWindowTextW(rId);
	if (rId.IsEmpty())
	{
		return std::nullopt;
	}
	auto id_str = Util::from_cstring(rId);
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
	editCtrl.SetWindowTextW(Util::to_cstring(pk));
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
		editCtrl.SetWindowTextW(Util::to_cstring(record->*key));
	}
	std::optional<Table> GetIdToCurrent()
	{
		record = getCurrent<Table>(editCtrl);
	}
};

#endif
