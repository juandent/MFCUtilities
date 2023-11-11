#pragma once




#include <sqlite_orm/sqlite_orm.h>

#include "PersistableClasses2.h"

#include "DateBinding.h"

inline 	auto& get_storage2()
{
	using namespace sqlite_orm;
	using namespace std;

	static int flag = 0;

	constexpr const char* db_name{ "With_Chrono_sys_days.sqlite" };


	static auto storage =
		make_storage(db_name,
			make_table("Claims",
				make_column("id_claim", &Claim::id, autoincrement(), primary_key()),
				make_column("fkey_patient", &Claim::fkey_patient),
				make_column("fkey_doctor", &Claim::fkey_doctor),
				make_column("fkey_medication", &Claim::fkey_medication),
				make_column("start_date", &Claim::start_date),
				make_column("submission_date", &Claim::submission_date),
				make_column("fkey_medication_2", &Claim::fkey_medication),
				make_column("asprose_claim_number", &Claim::asprose_claim_number),
				make_column("asprose_case_number", &Claim::asprose_case_number),
				make_column("asprose_amount_presented", &Claim::asprose_amount_presented),
				make_column("ins_claim_number", &Claim::ins_claim_number),
				make_column("asprose_acknowledgement_type", &Claim::asprose_acknowledgement_type),
				make_column("comment", &Claim::comment),
				make_column("amount", &Claim::amount),
				make_column("other_system_id", &Claim::other_system_id),
				make_column("status", &Claim::status)));

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
	using Storage_t = decltype(get_storage2());

	Storage() = delete;
	// static void initialize();
	static Storage_t& getStorage() { return get_storage2(); }
	static void fill_db_with_test_data();
	// static void empty_database();
	// static void upgrade_database();
};

