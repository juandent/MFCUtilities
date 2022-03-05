#pragma once

#include <string>

// Storage && persistent classes
#include "../Model/Model.ORM.Definitions.h"

// controller interfaces
#include "../Controllers/Controller.Interfaces.h"

// repositories:
#include "TesterForMFCUtilitiesDLL/Repositories/StatementLineRepository.h"
#include "TesterForMFCUtilitiesDLL/Repositories/CategoryRepository.h"

using namespace std::string_literals;

class DbWideOps
{
public:
	void EmptyDb()
	{
		ORM::Storage::empty_database();
	}
	void FillDbWithSampleData()
	{
		ORM::Storage::fill_db_with_test_data();
	}
};

#if 0
class UtilityOps
{
public:
	void LoadCategories(Controller::ICategoryLoader& loader)
	{
		loader.load("C:\\Users\\Juan Dent\\Source\\Repos\\TransformDownloadedStatements\\ExpenseManagerMFC\\Category.cpp"s);
		loader.store();
	}
};
#endif

template<typename StatementLineRepoType = StatementLineRepository,
typename CategoryRepoType = CategoryRepository>
class UnitOfWork
{
public:

	UnitOfWork()
	{
		ORM::Storage::getStorage().begin_transaction();
	}
	// repositories:
	StatementLineRepoType		repoStatementLine;
	CategoryRepoType			repoCategory;

	// Database wide operations:
	DbWideOps dbWideOps;

	// Utility operations:
	///UtilityOps utilityOps;
	
	// commit and restart transaction:
	// returns: 0 if all ok
	int Complete()
	{
		ORM::Storage::getStorage().commit();
		ORM::Storage::getStorage().begin_transaction();
		return 0;
	}
};
