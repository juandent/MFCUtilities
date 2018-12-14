#include "stdafx.h"
#include "Controller.LoadCompoundDocIntoDB.h"
#include "../Model/Model.ORM.Definitions.h"
#include "CSVFile.h"


namespace Controller
{
	using namespace Model;
	using namespace sqlite_orm;
	using namespace std;


	LoadCompoundDocIntoDB::LoadCompoundDocIntoDB(const std::string & file_name)
	{
		m_file_reader.load(file_name);
	}

	LoadCompoundDocIntoDB::~LoadCompoundDocIntoDB()
	{
	}

	void LoadCompoundDocIntoDB::DoLoadIntoDB()
	{
		auto statement_date = m_file_reader.getDate({ 1,0 });


	}

}