#include "stdafx.h"


#include "Model.ORM.Definitions.h"
#include "Model.DateBinding.h"

namespace Model
{

	void ORM_Central::fill_db()
	{
		using namespace sqlite_orm;
		using namespace date;
		using namespace std;

		auto& storage = refresh_and_get_schema();

		Person leslie{ 1, "Leslie"s, "Hulse"s };
		Person juan{ 2, "Juan"s, "Dent"s };

		Category health{ "Salud", false };
		Category gas{ "Gasolina", false };
		Category supermarket{ "Supermercado"s, false };

		Account cred{ "3777-11**-****-7645", 2, "", Coin::Both, "AM Cashback"s, AccountType::CreditCard };
		Account bank{ "903343622", 2, "", Coin::Dolar, "Dollars main"s, AccountType::BankAccount };
		Account cred_leslie{ "5491-94**-****-2718", 1,"", Coin::Both, "MC Gane Premios"s, AccountType::CreditCard };

		Account payment_to{ "4590"s, 1, "", Coin::Dolar, "????", AccountType::BankAccount };

		Concept conc{ "Automercado"s, "Supermercado"s, nullptr, true, false };

		auto n = std::chrono::system_clock::now().time_since_epoch().count();
		auto start = date::sys_days{ days{n} };

		year_month_day ymd{ year{2018}, month{8}, day{21} };
		sys_days tod = ymd;
		sys_days ttod = tod + days{ 1 };

		auto today = tod.time_since_epoch().count();

		StatementLine line1{ -1,
			"3777-11**-****-7645"s,
			tod,
			"Automercado"s,
			389045000.50,
			2378.99,
			make_shared<std::string>("Supermercado"s),
			true,
			"Paseo a Bungalows"s,
			nullptr, //"4590"s,
			ttod
		};

		Statement  st {
			"SomeFile.csv"s,
			"C:\\Users\\Juan Dent\\Downloads\\"s,
			tod,
			"3777-11**-****-7645"s
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
			auto line = storage.get<StatementLine>(line1.m_id);
			auto act = storage.get<Account>("3777-11**-****-7645");
			if (auto act2 = storage.get_no_throw<Account>("3777-11**-****"))
			{

			}
			auto res = storage.get_all<StatementLine>(where(c(&StatementLine::m_account_fid) == "3777-XXXXXX-X6745"s));
			assert(res.size() == 0);
			auto str = SysDaysToString(line.m_lineDate);
			auto num = std::to_string(3);
		}
		catch(std::runtime_error& err)
		{
			auto wh = err.what();
			int i = 0;
		}

	}

	void ORM_Central::remove_all_from_database()
	{
		using namespace sqlite_orm;
		using namespace date;
		using namespace std;

		auto& storage = refresh_and_get_schema();
		try
		{
			storage.remove_all<LineResponsibility>();
			storage.remove_all<StatementLine>();
			storage.remove_all<Concept>();
			storage.remove_all<Category>();
			storage.remove_all<Account>();
			storage.remove_all<Responsible>();
			storage.remove_all<Person>();

		}
		catch (std::system_error& exc)
		{
			auto what = exc.what();
			int i = 9;
		}
	}
}


