#include "stdafx.h"

#include "CategoryRepository.h"

using namespace std::string_literals;

template<typename SortedBy, typename C>
C CategoryRepository::allCategoriesSortedBy(SortedBy sortedBy)
{
	using namespace sqlite_orm;
	using namespace Model;

	auto collection = ORM::Storage::getStorage().get_all<Model::Category>(sortedBy);
	return collection;
}

void CategoryRepository::Utility::LoadCategories(Controller::ICategoryLoader & loader)
{
	loader.load("C:\\Users\\Juan Dent\\Source\\Repos\\TransformDownloadedStatements\\ExpenseManagerMFC\\Category.cpp"s);
	loader.store();
}
