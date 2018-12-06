#include "stdafx.h"


#include "Model.ORM.Definitions.h"
#include "Model.DateBinding.h"

#include "../Controllers/Controller.CategoryLoader.h"
#include "../Repositories/CategoryRepository.h"
#include "../Repositories/StatementLineRepository.h"

namespace ORM
{
	using namespace Model;

	void Storage::initialize()
	{
		Storage::empty_database();
		CategoryRepository repo;
		repo.utility.LoadCategories(Controller::CategoryLoader{});
		fill_db_with_test_data();
	}

	void Storage::fill_db_with_test_data()
	{
		using namespace sqlite_orm;
		using namespace date;
		using namespace std;

		auto& storage = Storage::getStorage();

		Person leslie{ 1, "Leslie"s, "Hulse"s };
		Person juan{ 2, "Juan"s, "Dent"s };
		Person jurocama{ 3, ""s, ""s, "JUROCAMA S.A." };

		Category health{ "Salud", false };
		Category gas{ "Gasolina", false };
		Category supermarket{ "Supermercado"s, false };

		Account cred{ "3777-11**-****-7645", 2, "", Coin::Both, "AM Cashback"s, AccountType::CreditCard };
		Account bank{ "903343622", 2, "", Coin::Dolar, "Dollars main"s, AccountType::BankAccount };
		Account cred_leslie{ "5491-94**-****-2718", 1,"", Coin::Both, "MC Gane Premios"s, AccountType::CreditCard };

		Account payment_to{ "4590"s, 1, "", Coin::Dolar, "????", AccountType::BankAccount };
		Account jurocamaAct{ "15100-01**-****-8336", 3, "", Coin::Dolar, "", AccountType::BankAccount };

		Concept conc{ "Automercado"s, "Supermercado"s, nullptr, true, false };
		auto jurocamaId = Storage::wrap(jurocamaAct.m_number_id);
		int count = jurocamaId.use_count();
		Concept conceptJurocama{ "TFT-SINPE A: 15100-01**-****-8336"s,"PAGO A JUROCAMA"s, jurocamaId, true, false };
		count = jurocamaId.use_count();

		auto n = std::chrono::system_clock::now(); // .time_since_epoch().count();
		auto since_epoch = n.time_since_epoch();	// a duration in seconds
		since_epoch -= std::chrono::hours{ 6 };

		//using days = std::chrono::duration
		//	<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

		using dur_days = std::chrono::duration<int, std::ratio<3600*24, 1>>;

		auto dur_in_days_ = std::chrono::duration_cast<dur_days>(since_epoch);

		year_month_day now = date::sys_days{ days{dur_in_days_} };
		int y = static_cast<int>( now.year());
		int m = (unsigned)now.month();
		int d = (unsigned)now.day();
		sys_days now_dp = now;

		string dayName = JD::weekDay(now_dp);

		
		//auto x = sun[last]/may/2018;
		auto xx = 2015_y / sep / 25_d;
		sys_days xx_days = xx;
		int days_since_epoch = xx_days.time_since_epoch().count();


		year_month_day yymmdd = 2018_y /8 / 21_d;

		cout << yymmdd << endl;

		auto plusmonth = yymmdd + months{ 1 };
		auto minusyear = yymmdd - years{ 1 };

		auto nextmonth = 2015_y / month{ 1 } / 31 + months{ 1 };
		if (!nextmonth.ok())
		{
			year_month_day next = nextmonth.year() / nextmonth.month() / last_spec{};
			ostringstream os;
			os << next;
			auto str = os.str();
			int i = 0;


			year_month_day overflow = sys_days{ nextmonth };
			int j;
		}
		year_month_day yymd = 2018_y / month{ 12 } / fri[4];
		sys_days yymd_dp = yymd;
		string ss = JD::weekDay(yymd_dp);


		year_month_day ymd{ year{2018}, month{8}, day{21} };
		sys_days tod = ymd;
		sys_days ttod = tod + days{ 1 };

		auto today = tod.time_since_epoch().count();

		// time_point:
		auto local_time = date::make_zoned(date::current_zone(), std::chrono::system_clock::now());
		auto local_time_ = local_time.get_local_time();
		auto sys_time = local_time.get_sys_time();
		
		//sys_days lt = local_time_;


		sys_days dp = std::chrono::time_point_cast<days>(std::chrono::system_clock::now());
		year_month_day ymd__ = dp;

		StatementLine line1{ -1,
			"3777-11**-****-7645"s,
			tod,
			"Automercado"s,
			389045000.50,
			2378.99,
			Storage::wrap(supermarket.m_name_id),
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
		storage.replace(jurocama);

		// Categories
		storage.replace(health);
		storage.replace(gas);
		storage.replace(supermarket);

		// Accounts
		storage.replace(cred);
		storage.replace(bank);
		storage.replace(payment_to);
		storage.replace(jurocamaAct);

		auto pers = cred.getPerson();


		// Concepts
		storage.replace(conc);
		storage.replace(conceptJurocama);

		auto assoc_account = conc.getAccount();
		auto assoc_account2 = conceptJurocama.getAccount();
		if (assoc_account2 != nullptr)
		{
			auto pers = assoc_account2->getPerson();
			auto cmp = pers.m_company_name;
			int i = 0;
		}

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

	void Storage::empty_database()
	{
		using namespace sqlite_orm;
		using namespace date;
		using namespace std;

		auto& storage = Storage::getStorage();
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


