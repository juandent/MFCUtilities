#pragma once

#include "Data.h"

template<typename MappedType, int MappedType::*PKey>
class DbSet
{
	std::vector<std::shared_ptr<MappedType>> m_added_records;
public:
	void Add(std::shared_ptr<MappedType> record)
	{
		m_added_records.push_back(record);
	}

	void SaveChanges()
	{
		auto& storage = Storage::getStorage();

		storage.transaction([]() {

			for (auto r : m_added_records)
			{
				if (r->*PKey == -1)
				{
					storage.insert(*r);
				}
				else
				{
					storage.replace(*r);
				}
			}
			});
	}
};

