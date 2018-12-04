#pragma once


#include "../Model/Model.DataTier.h"
#include <vector>
#include <memory>
#include <algorithm>
#include "../ORM/ORM.Repository.h"
#include "../Controllers/Controller.Interfaces.h"


class CategoryRepository : public ORM::Repository<Model::Category>
{
public:

	template<typename SortedBy, typename C = std::vector<Model::Category>>
	C allCategoriesSortedBy(SortedBy sortedBy);

	struct Utility
	{
		void LoadCategories(Controller::ICategoryLoader& loader);
	} utility;
};
