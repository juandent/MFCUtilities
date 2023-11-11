#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <iostream>
#include  <chrono>

#include <..\ORM_Extensions\DateBinding.h>

using namespace sqlite_orm;
using namespace std::literals;
using namespace std::chrono;
using namespace std::chrono_literals;

struct Person
{
	int id;
	std::string first_name;
	std::string last_name;
	std::optional<sys_days>    birth_date;
	std::optional<std::string> country;
};

template<bool first_version>
auto create_storage()
{

	if constexpr (first_version == true) {
		::remove("add_remove.sqlite");

		auto storage1 = make_storage("add_remove.sqlite",
			make_table("People",
				make_column("id", &Person::id, primary_key(), autoincrement()),
				make_column("first", &Person::first_name),
				make_column("last", &Person::last_name),
				make_column("birth", &Person::birth_date)));
		return storage1;
	}
	else {   // remove + add columns

		auto storage2 = make_storage("add_remove.sqlite",
			make_table("People",
				make_column("id", &Person::id, primary_key(), autoincrement()),
				make_column("first", &Person::first_name),
				make_column("last", &Person::last_name),
				make_column("country", &Person::country)));
		return storage2;
	}
}


int main()
{
	auto store1 = create_storage<true>();

	store1.sync_schema();

	auto les_birth = 1968y / September / 21d;
	auto jd_birth = 1960y / July / 26d;

	std::vector<Person> people = {
		Person {1, "Leslie", "Hulse", les_birth, "USA"},
		Person {2, "Juan", "Dent", jd_birth, "Costa Rica"}
	};

	store1.replace_range(people.begin(), people.end());

	auto vec = store1.get_all<Person>();

	auto store2 = create_storage<false>();

	auto res = store2.sync_schema();
	
	for (auto& [first, second] : res)
	{
		assert(second == sync_schema_result::new_columns_added_and_old_columns_removed);
	}

	auto vec2 = store2.get_all<Person>();
}

