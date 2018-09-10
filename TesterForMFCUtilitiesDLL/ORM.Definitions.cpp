#include "stdafx.h"

#include "DataTier.h"
#include "ORM.Definitions.h"

namespace ORM
{
	auto& ORM_DataTier::get_storage()
	{
		using namespace sqlite_orm;
		using namespace date;
		using namespace std;
		using namespace DataTier;

		static int flag = 0;
		static auto storage = make_storage("statements.sqlite",
			make_table("CATEGORIES"s,
				make_column("name",
					&Category_Rec::m_name_id,
					primary_key()),
				make_column("is_real",
					&Category_Rec::m_real_expense_or_income)),
			make_table("PERSONS"s,
				make_column("id",
					&Person_Rec::m_id,
					primary_key()),
				make_column("first_name",
					&Person_Rec::m_first_name),
				make_column("last_name",
					&Person_Rec::m_last_name),
				make_column("company_name",
					&Person_Rec::m_company_name)),
			make_table("ACCOUNTS"s,
				make_column("number",
					&Account_Rec::m_number_id,
					primary_key()),
				make_column("owner",
					&Account_Rec::m_owner_fid),
				make_column("currency",
					&Account_Rec::m_currency),
				foreign_key(&Account_Rec::m_owner_fid).references(&Person_Rec::m_id)),
			make_table("CONCEPTS"s,
				make_column("id",
					&Concept_Rec::m_concept_id,
					primary_key()),
				make_column("category",
					&Concept_Rec::m_category_name_fid),
				make_column("account_payment",
					&Concept_Rec::m_account_payment_fid),
				make_column("always",
					&Concept_Rec::m_always),
				make_column("is_regex",
					&Concept_Rec::m_is_regex),
				foreign_key(&Concept_Rec::m_category_name_fid).references(&Category_Rec::m_name_id),
				foreign_key(&Concept_Rec::m_account_payment_fid).references(&Account_Rec::m_number_id)
			),
			make_table("RESPONSIBLES"s,
				make_column("id",
					&Responsible_Rec::m_id,
					primary_key()),
				make_column("person",
					&Responsible_Rec::m_person_fid),
				make_column("percentage",
					&Responsible_Rec::m_percentage),
				foreign_key(&Responsible_Rec::m_person_fid).references(&Person_Rec::m_id)
			),
			make_table("LINE_RESPONSABILITIES"s,
				make_column("statement_id",
					&LineResponsibility_Rec::m_statement_fid),
				make_column("responsible_id",
					&LineResponsibility_Rec::m_responsible_fid),
				foreign_key(&LineResponsibility_Rec::m_statement_fid).references(&StatementLine_Rec::m_id),
				foreign_key(&LineResponsibility_Rec::m_responsible_fid).references(&Responsible_Rec::m_id)
			),
			make_table("STATEMENTLINES"s,
				make_column("id",
					&StatementLine_Rec::m_id,
					primary_key()),
				make_column("account",
					&StatementLine_Rec::m_account_fid),
				make_column("line_date",
					&StatementLine_Rec::m_lineDate),
				make_column("concept",
					&StatementLine_Rec::m_concept_fid),
				make_column("colones"s,
					&StatementLine_Rec::m_amountInLocal),
				make_column("dolares"s,
					&StatementLine_Rec::m_amountInDollars),
				make_column("category"s,
					&StatementLine_Rec::m_category_fid),
				make_column("enabled",
					&StatementLine_Rec::m_enabled),
				make_column("details",
					&StatementLine_Rec::m_details),
				make_column("payment_to",
					&StatementLine_Rec::m_payment_to_fid),
				foreign_key(&StatementLine_Rec::m_account_fid).references(&Account_Rec::m_number_id),
				foreign_key(&StatementLine_Rec::m_category_fid).references(&Category_Rec::m_name_id),
				foreign_key(&StatementLine_Rec::m_concept_fid).references(&Concept_Rec::m_concept_id),
				foreign_key(&StatementLine_Rec::m_payment_to_fid).references(&Account_Rec::m_number_id)
			));

			if (flag == 0)
			{
				flag = 1;
				storage.sync_schema();
			}
			return storage;
	}

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



		auto line = storage.get<StatementLine_Rec>(line1.m_id);
		auto act = storage.get<Account_Rec>("3777-XXXXXX-X6745");
		if (auto act2 = storage.get_no_throw<Account_Rec>("3777-XXXXXX"))
		{

		}

		auto str = SysDaysToString(line.m_lineDate);
		auto num = std::to_string(3);

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
#if 0
	void fillDB()
	{
		using namespace DataTier;
		using namespace date;
		using namespace std;

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
		catch(std::system_error& exc)
		{
			auto what = exc.what();
			int i = 9;
		}
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


#if 0
  auto rows = storage.select(columns(&StatementLine_Rec::m_id,
			&StatementLine_Rec::m_account_fid,
			&StatementLine_Rec::m_lineDate,
			&StatementLine_Rec::m_concept_fid,
			&StatementLine_Rec::m_amountInLocal,
			&StatementLine_Rec::m_amountInDollars,
			&StatementLine_Rec::m_details));
#endif


		auto line = storage.get<StatementLine_Rec>(line1.m_id);
		auto act = storage.get<Account_Rec>("3777-XXXXXX-X6745");
		if( auto act2 = storage.get_no_throw<Account_Rec>("3777-XXXXXX"))
		{
			
		}
		
		auto str = SysDaysToString(line.m_lineDate);
		auto num = std::to_string(3);
		
			// ,
		//	multi_order_by(order_by(&Employee::name).asc(), order_by(&Employee::salary).desc()));
#if 0
  for (auto& r : rows)
		{
			auto a = std::get<0>(r);
			auto b = std::get<1>(r);
			auto c = std::get<2>(r);
			auto d = std::get<3>(r);
			auto e = std::get<4>(r);
			auto f = std::get<5>(r);
			auto g = std::get<6>(r);
			int i = 0;
		}
#endif



	}
#endif
}


