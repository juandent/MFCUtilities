

#include "stdafx.h"

#include "Data_Tier.h"

#if 1

void Storage::initialize()
{
	// initialize tz library asynchronously
	std::thread{ date::get_tzdb }.detach();

	//Storage::empty_database();
	//CategoryRepository repo;
	//repo.utility.LoadCategories(Controller::CategoryLoader{});
	// do this one time at the start to increase performance
	//auto zoned_time_ = date::make_zoned(date::current_zone(), std::chrono::system_clock::now());
	//fill_db_with_test_data();
}

///////////////////////////////////
/// Order:
///
/// Categoria
/// Pais
/// Banco
/// AccountOwner
/// Account
/// Concepto
/// StatementLine
/// Transaccion
/// 
/// 


void Storage::fill_db_with_test_data()
{
	using namespace sqlite_orm;
	using namespace date;
	using namespace std;


	auto& storage = Storage::getStorage();

#if WORKS
	storage.remove_all<Transaccion>();
	storage.remove_all<Categoria>();
	storage.remove_all<StatementLine>();
	storage.remove_all<Concepto>();
	storage.remove_all<Account>();
	storage.remove_all<AccountOwner>();
	storage.remove_all<Banco>();
	storage.remove_all<Pais>();
#else
	storage.remove_all<Transaccion>();
	storage.remove_all<StatementLine>();
	storage.remove_all<Categoria>();
	storage.remove_all<Concepto>();
	storage.remove_all<Account>();
	storage.remove_all<AccountOwner>();
	storage.remove_all<Banco>();
	storage.remove_all<Pais>();

#endif


	

	year_month_day ymd{ year{2018}, month{8}, day{21} };
	sys_days tod = ymd;
	sys_days ttod = tod + days{ 1 };


	Categoria cat{ -1, "Kisha", true };
	cat.id_categoria = storage.insert(cat);
	
	Pais pais{ -1, "Costa Rica" };
	pais.id_pais = storage.insert(pais);

	Banco banco{ -1, "BAC San Jose", "Barrio Dent", pais.id_pais };
	banco.id_bank = storage.insert(banco);

	Banco banco_dest{ -1, "BCR", "San Jose", pais.id_pais };
	banco_dest.id_bank = storage.insert(banco_dest);

	Banco banco_other{ -1, "BNCR", "San Jose", pais.id_pais };
	banco_other.id_bank = storage.insert(banco_other);


	AccountOwner owner{ -1, "Juan Dent Herrera" };
	owner.id_owner = storage.insert(owner);

	AccountOwner other{ -1, "Leslie Rachel Hulse" };
	other.id_owner = storage.insert(other);


	Account act{ -1, "3777-11****-*7645", banco.id_bank, owner.id_owner, "AMEX Cashback Premium",true };
	act.id_account = storage.insert(act);
	
	Account actdest{ -1, "CR73400889112311", banco_dest.id_bank, owner.id_owner, "Cuenta de ahorros",false};
	actdest.id_account = storage.insert(actdest);

	Account actother{ -1, "CR310*********8467", banco_other.id_bank, other.id_owner, "Para platas de Leslie", false };
	actother.id_account = storage.insert(actother);

	
	Concepto con{ -1, "TFT-SINPE A: 15103-02**-****-8467", actother.id_account };
	con.id_concepto = storage.insert(con);
	

	StatementLine line{ -1, tod, "sss", con.id_concepto,cat.id_categoria };
	line.id_statement_line = storage.insert(line);
	

	//auto ac = make_unique<int>(actother.id_account);
	
	//auto acnul = make_shared<int>(nullptr);
	
	Transaccion trans{ -1, 1200, 50, act.id_account, actother.id_account, line.id_statement_line };
	trans.id_transaccion = storage.insert(trans);

	if(trans.fkey_account_destination)
	{
		int a = *trans.fkey_account_destination;
		int i = 0;
	}

	Transaccion trans2{ -1, 1500, 0, act.id_account, nullopt, line.id_statement_line };
	trans2.id_transaccion = storage.insert(trans2);

//	StatementLine line{ -1, ttod, "Gas", -1, 20000, 10, -1 };
//	storage.insert(line);


	//  If you want to know the total amount of salary on each customer, then GROUP BY query would be as follows:
//  SELECT NAME, SUM(SALARY)
//  FROM COMPANY
//  GROUP BY NAME;
	// auto amount_colones = storage.select(columns(&StatementLine::description, sum(&StatementLine::amount_colones)), group_by(&StatementLine::description));
	// for (auto& t : amount_colones) {
	// 	ostringstream oss;
	//
	// 	oss << std::get<0>(t) << '\t' << *std::get<1>(t) << endl;
	// 	auto s = oss.str();
	// 	int i = 0;
	// }
	ostringstream oss;
	oss  << std::numeric_limits<long double>::digits10 << std::endl;											// 123,456,789,012,345
	auto s = oss.str();
	oss.clear();
	oss << std::numeric_limits<double>::digits10 << std::endl;													// 1,234,567,890,123.45
	s = oss.str();

	constexpr auto m  = std::numeric_limits<double>::max_digits10;

	double d = 123456789012345.67;
	auto ss = std::to_string(d);
	d *= -1;
	ss = std::to_string(d);
	
	
#if 0
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

	Concept conc{ "Automercado"s, /*Nullable::make_nullable( "Supermercado"s),*/ nullptr, true, false };
	auto jurocamaId = Nullable::make_nullable(jurocamaAct.m_number_id);
	int count = jurocamaId.use_count();
	Concept conceptJurocama{ "TFT-SINPE A: 15100-01**-****-8336"s, /*Nullable::make_nullable( "PAGO A JUROCAMA"s),*/ jurocamaId, true, false };
	count = jurocamaId.use_count();

	auto n = std::chrono::system_clock::now();	// a time_point
	auto since_epoch = n.time_since_epoch();	// a duration in seconds
	auto dur_in_hours_0 = std::chrono::duration_cast<std::chrono::hours>(since_epoch);
	std::chrono::hours h = round<std::chrono::hours>(since_epoch);
	since_epoch -= std::chrono::hours{ 6 };
	auto dur_in_hours_1 = std::chrono::duration_cast<std::chrono::hours>(since_epoch);


	//using days = std::chrono::duration
	//	<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

	using dur_days = std::chrono::duration<int, std::ratio<3600 * 24, 1>>;

	auto dur_in_days_ = std::chrono::duration_cast<dur_days>(since_epoch);

	year_month_day now = date::sys_days{ days{dur_in_days_} };
	int y = static_cast<int>(now.year());
	int m = (unsigned)now.month();
	int d = (unsigned)now.day();
	sys_days now_dp = now;

	string dayName = JD::weekDay(now_dp);


	//auto x = sun[last]/may/2018;
	auto xx = 2015_y / sep / 25_d;
	sys_days xx_days = xx;
	int days_since_epoch = xx_days.time_since_epoch().count();


	year_month_day yymmdd = 2018_y / 8 / 21_d;

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
	ostringstream osss{};
	osss << yymd;
	string str = osss.str();
	sys_days yymd_dp = yymd;
	string ss = JD::weekDay(yymd_dp);


	year_month_day ymd{ year{2018}, month{8}, day{21} };
	sys_days tod = ymd;
	sys_days ttod = tod + days{ 1 };

	auto today = tod.time_since_epoch().count();

	// time_point:
	auto zoned_time_ = date::make_zoned(date::current_zone(), std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()));
	ostringstream oss{};
	oss << zoned_time_;
	string sss = oss.str();
	auto lt = zoned_time_.get_local_time();
	auto lt_days = floor<days>(lt);
	year_month_day ymd_local{ lt_days };

	{
		using namespace std::chrono;

		auto zoned_time_ = date::make_zoned(date::current_zone(), std::chrono::system_clock::now());
		auto lt = zoned_time_.get_local_time();
		auto lt_days = floor<days>(lt);
		year_month_day ymd_local{ lt_days };
		std::stringstream ss{};

		to_stream(ss, "%d/%m/%Y", lt_days);
		string str = ss.str();
		sys_time<seconds> tt;
		ss >> date::parse("%d/%m/%Y", tt);
		if (ss.fail())
			throw runtime_error("error parsing date");
		auto rr = format("%d/%m/%Y", tt);

		ss.flush();
		sys_time<days> tdays;
		to_stream(ss, "%d/%m/%Y", lt_days);
		string from_ss = ss.str();
		ss >> date::parse("%d/%m/%Y", tdays);
		if (ss.fail())
			throw runtime_error("error parsing date");
		auto rre = format("%d/%m/%y", tt);

		from_stream(ss, "%d/%m/%y", tt);
		//ss >> date::parse("%F", lt_days);

		auto ret = format("%F %T %Z", tt);
		int i = 0;
	}
	{
		using namespace std::chrono;
		auto zoned_time = date::make_zoned(date::current_zone(), std::chrono::system_clock::now());
		auto lt = zoned_time.get_local_time();
		auto lt_in_days = floor<days>(lt);
		auto ret = format("%d/%m/%y", lt_in_days);
		std::stringstream ss{ ret };
		sys_days tt;
		ss >> parse("%d/%m/%y", tt);
		auto r = format("%d/%m/%y", tt);
		int i = 0;
	}

	//sys_days lt = local_time_;


	sys_days dp = std::chrono::time_point_cast<days>(std::chrono::system_clock::now());
	year_month_day ymd__ = dp;

	StatementLine line1{ -1,
		cred.m_number_id,
		tod,
		"Automercado"s,
		389045000.50,
		2378.99,
		Nullable::make_nullable(supermarket.m_name_id),
		true,
		"Paseo a Bungalows"s,
		nullptr, //"4590"s,
		ttod
	};

	Statement  st{
		"SomeFile.csv"s,
		"C:\\Users\\Juan Dent\\Downloads\\"s,
		ttod
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

	auto pers = cred.getOwner();


	// Concepts
	storage.replace(conc);
	storage.replace(conceptJurocama);

	auto assoc_account = conc.getAccount();
	auto assoc_account2 = conceptJurocama.getAccount();
	if (assoc_account2 != nullptr)
	{
		auto pers = assoc_account2->getOwner();
		auto cmp = pers.m_company_name;
		int i = 0;
	}

	line1.m_id = storage.insert(line1);

	// Statements
	storage.replace(st);

	auto statement = line1.getStatement();

	auto vecStatementLines = supermarket.getStatementLines();

#if 1
	line1.AddResponsible(juan, 0.50);
	line1.AddResponsible(leslie, 0.50);

	auto v_resp = line1.getResponsibles();
	for (auto& resp : v_resp)
	{
		double percent = resp.m_percentage;
		Person person = resp.getPerson();
		string name = person.m_first_name + " " + person.m_last_name;
		int i = 0;
	}
#endif

	try
	{
		auto line = storage.get<StatementLine>(line1.m_id);
		auto act = storage.get<Account>("3777-11**-****-7645");
		if (auto act2 = storage.get_no_throw<Account>("3777-11**-****"))
		{

		}
		auto res = storage.get_all<StatementLine>(where(c(&StatementLine::m_belongs_to_account_fid) == "3777-XXXXXX-X6745"s));
		assert(res.size() == 0);
		auto str = SysDaysToString(line.m_lineDate);
		auto num = std::to_string(3);
	}
	catch (std::runtime_error& err)
	{
		auto wh = err.what();
		int i = 0;
	}
#endif

}

void Storage::empty_database()
{
	
}

#endif

