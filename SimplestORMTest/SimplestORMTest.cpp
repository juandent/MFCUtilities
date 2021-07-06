// SimplestORMTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <optional>
#include <iostream>

#include <sqlite_orm/sqlite_orm.h>

// #include <catch2/catch.hpp>
// #include <date/date.h>


struct Claim
{
	int get_id() const
	{
		return id;
	}

	void set_id(int id)
	{
		this->id = id;
	}

	int id;
	int fkey_patient;
	int fkey_doctor;
	int fkey_medication;					// for chronic meds
	int start_date;
	int submission_date;
	int asprose_claim_number;				// Claim: 85481
	std::string asprose_case_number;		// Case : 21INSENE0085
	double asprose_amount_presented;		// in colones
	std::string ins_claim_number;			// 202001017481-ACCSA0120094110
	int asprose_acknowledgement_type;		// full, partial, none: TODO: create an enum instead
	std::string comment;
	double amount;
};


struct Patient
{
	int get_id() const
	{
		return id;
	}

	void set_id(int id)
	{
		this->id = id;
	}

	std::string get_first_name() const
	{
		return first_name;
	}

	void set_first_name(const std::string& first_name)
	{
		this->first_name = first_name;
	}

	std::string get_last_name() const
	{
		return last_name;
	}

	void set_last_name(const std::string& last_name)
	{
		this->last_name = last_name;
	}

private:
	int id;
	std::string first_name;
	std::string last_name;
};

struct Doctor
{
	int id;
	std::string first_name;
	std::string last_name;
	int fkey_specialty;
};

struct Specialty
{
	int id;
	std::string name;
};

struct Medication
{
	int id;
	std::string name;
};

enum class InvoiceType
{
	Appointment = 1, Exam, Medication, Therapy, Procedure
};


struct Invoice
{
	int id;
	int fkey_claim;
	int number;
	double amount;
	int type;
	std::string description;
};


class Storage_Impl
{
public:
	Storage_Impl() = delete;	// prohibit instantiation
	static auto& get_storage();

	friend class Storage;

};


inline 	auto& Storage_Impl::get_storage()
{
	using namespace sqlite_orm;
	// using namespace date;
	using namespace std;

	static int flag = 0;

	constexpr const char* db_name{ "C:\\Users\\juan_\\OneDrive\\Health\\SeguroMedicoMFC\\Experiments_SeguroMedicoMFC.sqlite" };


	static auto storage =
		make_storage("",
			make_table("Claims",
				make_column("id_claim", &Claim::set_id, &Claim::get_id, autoincrement(), primary_key()),
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
				foreign_key(&Claim::fkey_patient).references(&Patient::get_id))
			,
			make_table("Patients",
				make_column("id_patient", &Patient::set_id, &Patient::get_id, autoincrement(), primary_key()),
				make_column("first_name", &Patient::set_first_name, &Patient::get_first_name, collate_nocase()),
				make_column("last_name", &Patient::set_last_name, &Patient::get_last_name, collate_nocase())),
			make_table("Doctors",
				make_column("id_doctor", &Doctor::id, autoincrement(), primary_key()),
				make_column("first_name", &Doctor::first_name, collate_nocase()),
				make_column("last_name", &Doctor::last_name),
				make_column("fkey_specialty", &Doctor::fkey_specialty),
				foreign_key(&Doctor::fkey_specialty).references(&Specialty::id))
			,
			make_table("Specialties",
				make_column("id_specialty", &Specialty::id, autoincrement(), primary_key()),
				make_column("name", &Specialty::name, collate_nocase()))
			,
			make_table("Invoices",
				make_column("id_invoice", &Invoice::id, autoincrement(), primary_key()),
				make_column("fkey_claim", &Invoice::fkey_claim),
				make_column("number", &Invoice::number),
				make_column("amount", &Invoice::amount),
				make_column("type", &Invoice::type),
				make_column("description", &Invoice::description),
				foreign_key(&Invoice::fkey_claim).references(&Claim::id)),
			make_table("Medications",
				make_column("id_medication", &Medication::id, autoincrement(), primary_key()),
				make_column("name", &Medication::name, collate_nocase())));
			

		
		
		
	

	if (flag == 0)
	{
		flag = 1;
		storage.sync_schema(false);
	}

	return storage;
}

int main()
{
	Patient p;
	p.set_first_name("Juan");
	p.set_last_name("Dent");
	
	p.set_id(Storage_Impl::get_storage().insert(p));

	Patient pt = Storage_Impl::get_storage().get<Patient>(p.get_id());
	
	
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
