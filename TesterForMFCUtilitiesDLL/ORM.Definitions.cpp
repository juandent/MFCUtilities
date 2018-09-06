#include "stdafx.h"

#include "DataTier.h"
#include "ORM.Definitions.h"

namespace ORM
{

	void fillDB()
	{
		using namespace DataTier;
		using namespace date;
		using namespace std;

		auto& storage = getStorage();

		storage.remove_all<StatementLine_Rec>();
		storage.remove_all<Category_Rec>();
		storage.remove_all<Account_Rec>();

		Person_Rec leslie{ 1, "Leslie"s, "Hulse"s };
		Person_Rec juan{ 2, "Juan"s, "Dent"s };

		Category_Rec health{ "Salud", false };
		Category_Rec gas{ "Gasolina", false };
		Category_Rec supermarket{ "Supermercado"s, false };

		Account_Rec cred{ "3777-XXXXXX-X6745", 1, Coin::Both };
		Account_Rec bank{ "88322", 2, Coin::Dolar };


		auto n = std::chrono::system_clock::now().time_since_epoch().count();
		

		auto start = date::sys_days{ days{n} };

		year_month_day ymd{ year{2018}, month{8}, day{21} };
		sys_days tod = ymd;

		auto today = tod.time_since_epoch().count();


		StatementLine_Rec line1{ -1,"3777-XXXXXX-X6745"s, tod, "Automercado"s,389045000.50, 2378.99, make_shared<std::string>("Supermercado"s), true,"Paseo a Bungalows" };

		storage.replace(health);
		storage.replace(gas);
		storage.replace(supermarket);
		storage.replace(cred);
		storage.replace(bank);
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
}


