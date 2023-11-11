#include "stdafx.h"

#include <regex>

#include "Controller.Interfaces.h"
#include "Controller.CategoryLoader.h"
#include "Controller.Main.h"


using namespace std;


void Controller::CategoryLoader::load(const std::string & file_name)
{
	m_file.load(file_name);
}

void Controller::CategoryLoader::store()
{
	ORM::Repository<Model::Category> repository{};

	for (int rcount = 9; rcount < m_file.getRowCount(); ++rcount)
	{
		auto& row = m_file.getRow(rcount);
		auto& col1 = row[0];

		regex remove{ "\t|\"" };
		auto& res = regex_replace(col1, remove, "");

		if(res == "};")
			break;
		
		Model::Category cat{ res, false };

		repository.Replace(cat);

		int i = 0;
	}
}
