#include "stdafx.h"
#include "Controller.Main.h"
#include <string>

using namespace std;

void Controller::MainController::load_categories(ICategoryLoader & loader)
{
	loader.load("C:\\Users\\Juan Dent\\Source\\Repos\\TransformDownloadedStatements\\ExpenseManagerMFC\\Category.cpp"s);
	loader.store();
}
