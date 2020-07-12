#pragma once

#include "../Model/Model.ORM.Definitions.h"

namespace ORM
{
	template <typename T>
	class Repository
	{
	public:
		using Element = T;

		template<typename ... Ids>
		T Get(Ids ... ids)
		{
			return Storage::getStorage().get<T>(ids...);
		}
		template<typename C = std::vector<T>>
		C GetAll()
		{
			return Storage::getStorage().get_all<T, C>();
		}
		template<typename C = std::vector<T>, typename U >
		C Find(U predicate)
		{
			return Storage::getStorage().get_all<T, C>(sqlite_orm::where(predicate));
		}
		int Insert(T entity)
		{
			return Storage::getStorage().insert(entity);
		}
		void InsertRange(std::vector<T> range)
		{
			Storage::getStorage().insert_range(range.begin(), range.end());
		}
		void Replace(T entity)
		{
			Storage::getStorage().replace(entity);
		}
		void ReplaceRange( std::vector<T> range)
		{
			Storage::getStorage().replace_range(range.begin(), range.end());
		}
		void Remove(T entity)
		{
			Storage::getStorage().remove<T>(entity);
		}
		void RemoveRange(std::vector<T> range)
		{
			Storage::getStorage().transaction([range = &range]
			{
				for (auto& obj : range)
				{
					Storage::getStorage().remove<T>(obj);
				}
				return true;
			});
		}
	
	public:
		Repository() {}
		~Repository() {}
	};

}