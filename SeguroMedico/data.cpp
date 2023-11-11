
#include "pch.h"
#include <sqlite_orm/sqlite_orm.h>

import fixed_point;

#include "Data.h"


using namespace sqlite_orm;

inline auto& storage = Storage::getStorage();


#if 0
template<typename OrderBy, typename OBy>
std::vector<Claim> get_all(OrderBy by, OBy oby)
{
	auto vec = storage.get_all<Claim>(multi_order_by(std::move(by), std::move(oby)));
	return vec;
}
#endif

template<typename T, typename WhereClause, typename ...OrderByClauses>
std::vector<T> load(WhereClause wclause, OrderByClauses&&...clauses)
{
	auto vec = storage.get_all<T>(where(wclause), multi_order_by(std::move(clauses)...));
	return vec;
}



template<typename OrderByClause>
std::vector<Claim> get_multi(OrderByClause clause)
{
	auto vec = storage.get_all<Claim>(clause);
	return vec;
}


void useStorage()
{
	//auto vec = storage.get_all<Claim>(order_by(&Claim::amount).desc());
	auto vec = storage.get_all<Claim>(where(true), multi_order_by(order_by(&Claim::amount).desc(), order_by(&Claim::start_date)));

	auto o1 = order_by(&Claim::amount).desc();
	auto o2 = order_by(&Claim::start_date);

	using o1_t = decltype(o1);
	using o2_t = decltype(o2);

	auto v = load<Claim>( c(&Claim::amount) < 10000, order_by(&Claim::amount).desc(), order_by(&Claim::id));
	// auto vv = get_all(o1,o2);

	auto y = get_multi(multi_order_by(order_by(&Claim::amount).desc(), order_by(&Claim::id)));
}


