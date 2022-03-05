#pragma once

#include <sqlite_orm/sqlite_orm.h>

#include "../Model/Model.ORM.Definitions.h"
#include "Controller.Interfaces.h"
#include "TesterForMFCUtilitiesDLL/Repositories/StatementLineRepository.h"

#if 0
namespace Controller
{

	template<typename StatementLineRepo = StatementLineRepository>
	class MainController
	{

		// configure MainController with repositories according to template parameters
		MainController()
		{
		}
	public:
		~MainController()
		{
		}

		StatementLineRepo statementLineRepository;

		void empty_database()
		{
			ORM::Storage::empty_database();
		}
		void fill_db_with_testdata()
		{
			ORM::Storage::fill_db_with_test_data();
		}
		void load_categories(ICategoryLoader& loader)
		{
			loader.load("C:\\Users\\Juan Dent\\Source\\Repos\\TransformDownloadedStatements\\ExpenseManagerMFC\\Category.cpp"s);
			loader.store();
		}
	public:
		static MainController getInstance()
		{
			static MainController instance;
			return instance;
		}
	};

}
#endif
