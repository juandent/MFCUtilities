

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
#include <map>

void Storage_Impl::copy_old_to_new()
{
	// starts full
	auto old = get_old_storage();
	// starts empty
	auto fresh = get_new_storage();


	auto lines = old.select(columns(
		alias_column<als_l>(&Location::id),
		upper(alias_column<als_l>(&Location::name)),
		alias_column<als_l>(&Location::url),
		alias_column<als_l>(&Location::email),
		alias_column<als_p>(&Password::id),
		alias_column<als_p>(&Password::password)),

		inner_join<als_p>(on( alias_column<als_p>(& Password::fkey_location) == c(alias_column<als_l>(& Location::id)))),

		where(true),

		order_by(alias_column<als_l>(&Location::name)).asc().collate_nocase());

	std::map<std::string, Location> name_to_location;
	std::map<std::string, std::vector<int>> m;	// location name -->> password ids

	for(const auto& line : lines )
	{
		static std::string url_concatenate{};
		static std::string email_concatenate{};

		Location loc{ -1, std::get<1>(line), std::get<2>(line), std::get<3>(line) };

		std::string name = std::get<1>(line);

		const int password_id = std::get<4>(line);
		auto s = m[name].size();
		if(s == 0)
		{
			url_concatenate = "";
			email_concatenate = "";
		}
		loc.url += url_concatenate;
		loc.email += email_concatenate;

		// MAP location name => Location
		name_to_location[name] = loc;

		m[name].push_back(password_id);
	}

	for (auto& [name, passwords] : m )
	{
		auto loc = name_to_location[name];
		loc.id = fresh.insert(loc);
		for (auto& pass_id : passwords)
		{
			int i = 0;
			auto pass = old.get<Password>(pass_id);
			pass.fkey_location = loc.id;
			fresh.insert(pass);
		}
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

	//storage.has_dependent_rows(p);

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



