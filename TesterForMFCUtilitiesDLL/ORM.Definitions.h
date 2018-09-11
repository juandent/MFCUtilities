#pragma once

#include "DataTier.h"

namespace ORM
{ 

	class ORM_DataTier
	{
	public:
		static auto& get_storage();
		void fill_db();
		void remove_all_from_database();

	};
}
