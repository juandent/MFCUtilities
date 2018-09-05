#include "stdafx.h"

#include "DataTier.h"
#include "ORM.Definitions.h"

namespace ORM
{

	void fillDB()
	{
		auto& storage = getStorage();

		storage.remove_all<StatementLine>();

		Category health{ "Health", false };
		Category gas{ "Gas", false };

		Account cred{ "3777-XXXXXX-X6745", Coin::Both };
		Account bank{ "88322", Coin::Dolar };


		auto n = std::chrono::system_clock::now().time_since_epoch().count();
		

		auto start = date::sys_days{ days{n} };

		year_month_day ymd{ year{2018}, month{8}, day{21} };
		sys_days tod = ymd;

		auto today = tod.time_since_epoch().count();


		StatementLine line1{ -1,"3777-XXXXXX-X6745", tod, "Automercado", L"¢445" , 389045000.50, 2378.99 };

		storage.replace(health);
		storage.replace(gas);
		storage.replace(cred);
		storage.replace(bank);
		line1.m_id = storage.insert(line1);

		auto rows = storage.select(columns(&StatementLine::m_id,
			&StatementLine::m_account,
			&StatementLine::m_lineDate,
			&StatementLine::m_concept,
			&StatementLine::m_amountInLocal,
			&StatementLine::m_amountInDollars,
			&StatementLine::m_wide));

		auto line = storage.get<StatementLine>(line1.m_id);

		
		auto str = SysDaysToString(line.m_lineDate);
		auto num = std::to_string(3);
		
			// ,
		//	multi_order_by(order_by(&Employee::name).asc(), order_by(&Employee::salary).desc()));
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


	}
}


