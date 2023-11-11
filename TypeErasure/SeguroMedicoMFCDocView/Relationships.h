#pragma once



template<typename G, int G::* Id, typename T, int T::* FKey>
std::optional<G> get_Card_1(T& t)
{
	// assert(t.*FKey >= 0);
	auto lines = storage.get_all<G>(where(c(Id) == t.*FKey));
	if (lines.size() == 0)
		return std::nullopt;
	return std::optional(lines.front());
}

template<typename G, int G::*FKey, typename T, int T::*Id, typename ... OrderClauses>
std::vector<G> get_Card_N( T& t, OrderClauses...clauses)
{
	auto lines = storage.get_all<G>(where(c(FKey) == t.*Id), multi_order_by(order_by(clauses)...));
	return lines;
}

template<typename G, typename...OrderClauses>
std::vector<G> get_All( OrderClauses... clauses)
{
	auto lines = storage.get_all<G>(multi_order_by(order_by(clauses)...));
	return lines;
}


template<typename G>
std::vector<G> get_All()
{
	auto lines = storage.get_all<G>();
	return lines;
}


// return storage.get_all<Invoice>(where(c(&Invoice::fkey_claim) == claim.id));


