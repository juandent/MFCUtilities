#pragma once


#include "../Model/Model.DataTier.h"
#include <vector>
#include <memory>
#include <algorithm>
#include "../ORM/ORM.Repository.h"
#include "../Controllers/Controller.Interfaces.h"

#if 0
template<typename C, typename D, template <typename Element> Container = std::vector>
Container<C> getSortedBy( D C::* field)
{
	using namespace sqlite_orm;
	using namespace Model;

	auto collection = ORM::Storage::getStorage().get_all<C>(order_by(field));
	return collection;
}
#endif
#if 1
template<typename ...Ts>
std::vector<Model::Category> getCategoriesSortedBy( Ts ... fields)
{
	using namespace sqlite_orm;
	using namespace Model;

	auto collection = ORM::Storage::getStorage().get_all<Model::Category>(multi_order_by(order_by(fields)...));
	return collection;
}
#endif

class CategoryRepository : public ORM::Repository<Model::Category>
{
public:

	template<typename SortedBy, typename C = std::vector<Model::Category>>
	C allCategoriesSortedBy(SortedBy sortedBy);

	template <class C, class D>
	std::vector<Model::Category> getCategoriesSortedBy( D C::* field)
	{
		using namespace sqlite_orm;
		using namespace Model;

		auto collection = ORM::Storage::getStorage().get_all<Model::Category>(order_by(field));
		return collection;
	}

	struct Utility
	{
		void LoadCategories(Controller::ICategoryLoader& loader);
	} utility;
};

template<typename SortedBy, typename C>
C CategoryRepository::allCategoriesSortedBy(SortedBy sortedBy)
{
	using namespace sqlite_orm;
	using namespace Model;

	auto collection = ORM::Storage::getStorage().get_all<Model::Category>(sortedBy);
	return collection;
}


#if 0
template<typename C, typename ... D>
std::vector<Model::Category> CategoryRepository::getCategoriesSortedBy((D C::* field)...)
{
	using namespace sqlite_orm;
	using namespace Model;

	auto collection = ORM::Storage::getStorage().get_all<Model::Category>(order_by(field));
	return collection;
}

template<typename D, typename C>
inline void getCategoriesSortedBy(D C::* field)
{
	bool eq = field == &Model::Category::m_name_id;
}
#endif
