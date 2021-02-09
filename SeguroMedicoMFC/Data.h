#pragma once
#undef min
#undef max

#include <sqlite_orm/sqlite_orm.h>
// #include <catch2/catch.hpp>
#include <date/date.h>


import Util;

// #include "../FixedPoint/Money.h"
#if 1
#include "..\TesterForMFCUtilitiesDLL/Model/Model.DateBinding.h"
#else
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Colones.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.CoinBinding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.Dolares.Binding.h"
#include "..\TesterForMFCUtilitiesDLL/Model/Model.AccountType.Binding.h"
#endif

#include "PersistentClasses.h"

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

	constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\Health\\SeguroMedicoMFC\\SeguroMedicoMFC.sqlite" };


	static auto storage =
		make_storage(db_name,
			make_table("Claims",
				make_column("id_claim", &Claim::id, autoincrement(), primary_key()),
				make_column("fkey_patient", &Claim::fkey_patient),
				make_column("fkey_doctor", &Claim::fkey_doctor),
				make_column("fkey_medication", &Claim::fkey_medication),
				make_column("start_date", &Claim::start_date),
				make_column("submission_date", &Claim::submission_date),
				make_column("asprose_claim_number", &Claim::asprose_claim_number),
				make_column("asprose_case_number", &Claim::asprose_case_number),
				make_column("asprose_amount_presented", &Claim::asprose_amount_presented),
				make_column("ins_claim_number", &Claim::ins_claim_number),
				make_column("asprose_acknowledgement_type", &Claim::asprose_acknowledgement_type),
				make_column("comment", &Claim::comment),
				make_column("amount", &Claim::amount),
				foreign_key(&Claim::fkey_doctor).references(&Doctor::id),
				foreign_key(&Claim::fkey_medication).references(&Medication::id),
				foreign_key(&Claim::fkey_patient).references(&Patient::id)),
			make_table("Patients",
				make_column("id_patient", &Patient::id, autoincrement(), primary_key()),
				make_column("first_name", &Patient::first_name, collate_nocase()),
				make_column("last_name", &Patient::last_name, collate_nocase())),
			make_table("Doctors",
				make_column("id_doctor", &Doctor::id, autoincrement(), primary_key()),
				make_column("first_name", &Doctor::first_name, collate_nocase()),
				make_column("last_name", &Doctor::last_name),
				make_column("fkey_specialty", &Doctor::fkey_specialty),
				foreign_key(&Doctor::fkey_specialty).references(&Specialty::id)),
			make_table("Specialties",
				make_column("id_specialty", &Specialty::id, autoincrement(), primary_key()),
				make_column("name", &Specialty::name, collate_nocase())),
			make_table("Medications",
				make_column("id_medication", &Medication::id, autoincrement(), primary_key()),
				make_column("name", &Medication::name, collate_nocase())),
			make_table("Invoices",
				make_column("id_invoice", &Invoice::id, autoincrement(), primary_key()),
				make_column("fkey_claim", &Invoice::fkey_claim),
				make_column("number", &Invoice::number),
				make_column("amount", &Invoice::amount),
				make_column("type", &Invoice::type),
				make_column("description", &Invoice::description),
				foreign_key(&Invoice::fkey_claim).references(&Claim::id)));


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

	Storage() = delete;
	static void initialize();
	static Storage_t& getStorage() { return Storage_Impl::get_storage(); }
	static void fill_db_with_test_data();
	static void empty_database();
};

//inline Storage::Storage_t& storage = Storage::getStorage();

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