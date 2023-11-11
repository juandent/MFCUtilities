#include "stdafx.h"

#include "CategoryRepository.h"

using namespace std::string_literals;

void CategoryRepository::Utility::LoadCategories(Controller::ICategoryLoader & loader)
{
	loader.load("C:\\Users\\Juan Dent\\Source\\Repos\\TransformDownloadedStatements\\ExpenseManagerMFC\\Category.cpp"s);
	loader.store();
}
