

#include "Data2.h"


#include <cassert>


void Storage::fill_db_with_test_data()
{
	using namespace sqlite_orm;
	// using namespace date;
	using namespace std;
	using namespace std::chrono;
	using namespace std::literals;


	auto& storage = get_storage2();

	year_month_day ymd{ year{2021}, month{5}, day{26} };
	sys_days d = ymd;

	Claim claim{ -1,1,1,1, d, d, "AAA", "90665", 230000, "20", 0, "comment", 230000, std::nullopt,  1 };

	claim.id = storage.insert(claim);

	auto claims = storage.get_all<Claim>();

	
}