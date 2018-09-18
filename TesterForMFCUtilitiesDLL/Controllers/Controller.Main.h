#pragma once


#include "../DataTier/DataTier.ORM.Definitions.h"
#include "Controller.Interfaces.h"


namespace Controller
{

	class MainController
	{
		ORM::ORM_Central orm_central;

		MainController()
		{
			orm_central.refresh_and_get_schema();
		}
	public:

		void empty_database()
		{
			orm_central.remove_all_from_database();
		}
		void fill_db_with_testdata()
		{
			orm_central.fill_db();
		}
		void load_categories(ICategoryLoader& loader);
	public:
		static MainController getInstance()
		{
			static MainController instance;
			return instance;
		}
	};

}