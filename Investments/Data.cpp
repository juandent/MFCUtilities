

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

	auto fondos = old.get_all<Fondo>();
	for (auto& record : fondos)
	{
		fresh.replace(record);
	}

	auto rendimientos = old.get_all<Rendimiento>();
	for (auto& record : rendimientos)
	{
		fresh.replace(record);
	}

	auto inversiones = old.get_all<Inversion>();
	for (auto& record : inversiones )
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


	empty_database();

	year_month_day ymd{ year{2021}, month{10}, day{13} };
	sys_days tod = ymd;
	sys_days daybefore = tod - days{ 1 };

	Fondo fondo{ -1, "inm1", "FCI", Fondo::trimestral };
	fondo.id = storage.insert(fondo);

	Inversion inv{ -1, 100, daybefore, fondo.id };
	inv.id = storage.insert(inv);

	Rendimiento rend{ -1, fondo.id, 7.45, tod };
	rend.id = storage.insert(rend);
}

void Storage::empty_database()
{
	using namespace sqlite_orm;
	using namespace std::chrono;
	using namespace std;


	auto& storage = Storage::getStorage();

	// order is vital!
	storage.remove_all<Rendimiento>();
	storage.remove_all<Inversion>();
	storage.remove_all<Fondo>();
}

////////////////////////////////////////
///DB access
///
/// 
int Inversion::num_participaciones_en(int fondo, std::chrono::year_month_day fecha) noexcept
{
	using namespace std::chrono;
	using namespace sqlite_orm;

	sys_days when = fecha;

	auto suma_participaciones = Storage::getStorage().select( sum(&Inversion::num_participaciones), where(c(&Inversion::beginning_date) <= when and (c(&Inversion::fkey_fondo) == fondo)), group_by(&Inversion::fkey_fondo));

	if( suma_participaciones.empty())
	{
		return 0;
	}

	auto&& rec = suma_participaciones[0];
	
	auto* suma = rec.get();

	auto sum_participaciones = static_cast<int>(* suma);

	return sum_participaciones;

}


