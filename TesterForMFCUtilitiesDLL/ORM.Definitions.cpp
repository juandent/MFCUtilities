#include "stdafx.h"

#include "DataTier.h"
#include "ORM.Definitions.h"

namespace ORM
{

	void ORM_DataTier::fill_db()
	{
		using namespace sqlite_orm;
		using namespace date;
		using namespace std;
		using namespace DataTier;

		auto& storage = get_storage();

		Person_Rec leslie{ 1, "Leslie"s, "Hulse"s };
		Person_Rec juan{ 2, "Juan"s, "Dent"s };

		Category_Rec health{ "Salud", false };
		Category_Rec gas{ "Gasolina", false };
		Category_Rec supermarket{ "Supermercado"s, false };

		Account_Rec cred{ "3777-XXXXXX-X6745", 1, Coin::Both };
		Account_Rec bank{ "88322", 2, Coin::Dolar };
		Account_Rec payment_to{ "4590"s, 1, Coin::Dolar };

		Concept_Rec conc{ "Automercado"s, "Supermercado"s, nullptr, true, false };

		auto n = std::chrono::system_clock::now().time_since_epoch().count();
		auto start = date::sys_days{ days{n} };

		year_month_day ymd{ year{2018}, month{8}, day{21} };
		sys_days tod = ymd;

		auto today = tod.time_since_epoch().count();

		StatementLine_Rec line1{ -1,
			"3777-XXXXXX-X6745"s,
			tod,
			"Automercado"s,
			389045000.50,
			2378.99,
			make_shared<std::string>("Supermercado"s),
			true,
			"Paseo a Bungalows"s,
			nullptr //"4590"s
		};

		// Persons
		storage.replace(leslie);
		storage.replace(juan);

		// Categories
		storage.replace(health);
		storage.replace(gas);
		storage.replace(supermarket);

		// Accounts
		storage.replace(cred);
		storage.replace(bank);
		storage.replace(payment_to);

		// Concepts
		storage.replace(conc);

		line1.m_id = storage.insert(line1);


		try
		{
			auto line = storage.get<StatementLine_Rec>(line1.m_id);
			auto act = storage.get<Account_Rec>("3777-XXXXXX-X6745");
			if (auto act2 = storage.get_no_throw<Account_Rec>("3777-XXXXXX"))
			{

			}
			auto str = SysDaysToString(line.m_lineDate);
			auto num = std::to_string(3);
		}
		catch(std::runtime_error& err)
		{
			auto wh = err.what();
			int i = 0;
		}

	}

	void ORM_DataTier::remove_all_from_database()
	{
		using namespace sqlite_orm;
		using namespace date;
		using namespace std;
		using namespace DataTier;

		auto& storage = get_storage();
		try
		{
			storage.remove_all<LineResponsibility_Rec>();
			storage.remove_all<StatementLine_Rec>();
			storage.remove_all<Concept_Rec>();
			storage.remove_all<Category_Rec>();
			storage.remove_all<Account_Rec>();
			storage.remove_all<Responsible_Rec>();
			storage.remove_all<Person_Rec>();

		}
		catch (std::system_error& exc)
		{
			auto what = exc.what();
			int i = 9;
		}
	}
}


