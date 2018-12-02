#pragma once


#include "../Model/Model.DataTier.h"
#include <vector>
#include <memory>
#include <algorithm>
#include "TesterForMFCUtilitiesDLL/ORM/ORM.Repository.h"


class CategoryRepository : public ORM::Repository<Model::Category>
{
public:

	template<typename C = std::vector<Model::Category>>
	C getCategoriesSortedByName()
	{
		using namespace sqlite_orm;
		using namespace Model;
		
		auto& collection = ORM::Storage::getStorage().get_all<Model::Category>(order_by(&Category::m_name_id));

		//auto& collection = Repository::GetAll();
		//std::sort(collection.begin(), collection.end(), [](Model::Category& lhs, Model::Category& rhs)
		//{
		//	return lhs.m_name_id < rhs.m_name_id;
		//});
		return collection;
	}

};
