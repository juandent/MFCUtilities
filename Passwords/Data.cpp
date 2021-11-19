

#include "pch.h"
#include "Data.h"



void Storage::initialize()
{
	// initialize tz library asynchronously
	std::thread{ std::chrono::get_tzdb }.detach();

	//fill_db_with_test_data();
//	empty_database();
}

void Storage::upgrade_database()
{
	Storage_Impl::copy_old_to_new();
}
///////////////////////////////////
/// Order for insert:
///
/// Location
/// Password
/// 
/// 

void Storage_Impl::copy_old_to_new()
{
	// starts full
	auto old = get_old_storage();
	// starts empty
	auto fresh = get_new_storage();

	auto locations = old.get_all<Location>();
	for (auto& record : locations)
	{
		fresh.replace(record);
	}

	auto passwords = old.get_all<Password>();
	for (auto& record : passwords)
	{
		fresh.replace(record);
	}
}

void Storage::backup_db()
{
	namespace fs = std::filesystem;

	auto path_to_db_name = fs::path(Storage_Impl::db_name);
	auto stem = path_to_db_name.stem().string();
	auto backup_stem = stem + "_backup1.sqlite";
	auto backup_full_path = path_to_db_name.parent_path().append(backup_stem).string();
	getStorage().backup_to(backup_full_path);
}


void Storage::fill_db_with_test_data()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;


	auto& storage = Storage::getStorage();

	// order is vital!
	storage.remove_all<Password>();
	storage.remove_all<Location>();

	year_month_day ymd{ year{2021}, month{10}, day{13} };
	sys_days tod = ymd;
	sys_days daybefore = tod - days{ 1 };



	Location l{ -1, "Banco Nacional", "www.bncr.fi.cr", "juandent@mac.com"};
	l.id = storage.insert(l);


	Password p{ -1, "JDHM0650", tod, l.id };
	p.id = storage.insert(p);

	Password p2{ -1, "JDHM", daybefore, l.id };
	p2.id = storage.insert(p2);


	auto pass = l.getPassword(ymd);

}

void Storage::empty_database()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;


	auto& storage = Storage::getStorage();

	// order is vital!
	storage.remove_all<Password>();
	storage.remove_all<Location>();
}

////////////////////////////////////////
///DB access
///
///

std::string Location::getPassword(std::chrono::year_month_day ymd)
{
	using namespace std::chrono;

	sys_days when = ymd;

	auto passwords = Storage::getStorage().get_all<Password>(where(c(&Password::begining_date) <= when), order_by(&Password::begining_date).desc());

	return passwords[0].password;
}



