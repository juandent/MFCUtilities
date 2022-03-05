

#include "Data.h"

#include <cassert>


void Storage::fill_db_with_test_data()
{
	using namespace sqlite_orm;
	// using namespace date;
	using namespace std;
	using namespace std::literals;


	auto& storage = get_storage();
	
	// order is vital!
	storage.remove_all<INSResponseLine>();
	storage.remove_all<Invoice>();
	storage.remove_all <INSResponse>();
	storage.remove_all<Claim>();
	storage.remove_all<Medication>();
	storage.remove_all<Doctor>();
	storage.remove_all<Specialty>();
	storage.remove_all<Patient>();

#if 0
	year_month_day ymd{ year{2018}, month{8}, day{21} };
	sys_days tod = ymd;
	sys_days ttod = tod - days{ 1 };
#endif

	Patient p{ -1, "Juan", "Dent" };
	p.id = storage.insert(p);

	Specialty s{ -1, "Ortopedista" };
	s.id = storage.insert(s);

	Doctor d{ -1, "Jaime", "Ulloa", s.id };
	d.id = storage.insert(d);
	
	Medication m{ -1, "True" };
	m.id = storage.insert(m);

	optional<int> oi = 2;

	Claim cl{ 1, p.id, d.id, m.id, ""s, ""s, 120000, ""s, 0, "comment"s,
		120000, oi, 0 };

	storage.replace(cl);

	assert(cl.id == 1);
	
	Claim cl2{ 2, p.id, d.id, m.id, ""s, ""s, 1200, ""s, 0, "comment"s,
	1200, oi, 0 };

	storage.replace(cl2);

	assert(cl2.id == 2);
	
	INSResponse ir{ 1, "8976543", 98777656665432, 25000.00, 0.0, 0.0, 5000.0, 0.0,20000, 0.0, 20000, "comments 1", 608.55 };
	storage.replace(ir);

	INSResponse ir2{ 2, "897000006543", 987776, 250.00, 0.0, 0.0, 50.0, 0.0,200, 0.0, 200, "comments 2", 612.55 };
	storage.replace(ir2);


	//optional<int> oi1 = 1;
	Invoice i{ -1, cl.id, "5555"s, 20000, 0, "desc"s, ir.id };		// fkey_INSResponse == 1
	i.id = storage.insert(i);

	assert(i.fkey_claim == 1);			// belongs to claim #1
	assert(i.fkey_INSResponse == 1);	//

	Invoice i2{ -1, cl2.id, "7777"s, 40000, 1, "desc 2"s, ir2.id };  // fkey_INSResponse == 2
	i2.id = storage.insert(i2);

	assert(i2.fkey_claim == 2);			// belongs to claim #2
	assert(i2.fkey_INSResponse == 2);	//
	
	auto lines = storage.select(columns(alias_column<als_c>(&Claim::id),
		select(greater_than(count<als_q>(), 0), from<als_q>(),
			where(c(alias_column<als_q>(&Invoice::fkey_INSResponse)) == 1 && c(alias_column<als_q>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)))),
		from<als_i>(), inner_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))), group_by(alias_column<als_c>(&Claim::id)));


	/*
	 * the previous select is serialized to the following SQL by sqlite_orm:
	 * 

	SELECT 'c'."id_claim", 
		(SELECT (COUNT(*) > ?) FROM 'Invoices' 'q' WHERE ( (('q'."fkey_INSResponse" = ?) AND ('q'."fkey_claim" = 'c'."id_claim"))) )
	FROM 'Invoices' 'i' INNER JOIN  'Claims' 'c' ON ('i'."fkey_claim" = 'c'."id_claim")  GROUP BY 'c'."id_claim" 

	*/


	for( auto& l : lines)
	{
		auto claim_id  = std::get<0>(l);
		auto col2 = std::get<1>(l);
		// auto greater_than_res = std::get<0>(col2);   // result of the function greater_than() in the nested select!
		//
		// if( claim_id == 1 )
		// {
		// 	assert(greater_than_res == true);	// THIS ASSERTION DOES NOT FAIL - here greater_than() returns correctly even though substitution of placeholders ? is probably broken!
		// }
		// else
		// {
		// 	assert(greater_than_res == false);	// THIS ASSERTION SHOULD NOT FAIL!! but it does, pointing to bug in substitution of placeholders ?
		// }

		int i = 0;
	}
}


