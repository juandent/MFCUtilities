#include "stdafx.h"

#include <regex>

#include "Controller.Interfaces.h"
#include "Controller.FileLoader.h"
#include "Controller.Main.h"


using namespace std;

void Controller::CategoryLoader::load(const std::string & file_name)
{
	m_file.load(file_name);
}

void Controller::CategoryLoader::store()
{
	for (int rcount = 9; rcount < m_file.getRowCount() /*&& rcount < 111*/; ++rcount)
	{
		auto& row = m_file.getRow(rcount);
		auto& col1 = row[0];

		regex remove{ "\t|\"" };
		auto& res = regex_replace(col1, remove, "");

		DataTier::Category cat{ res, false };

		ORM::ORM_Central::refresh_and_get_schema().replace(cat);

		int i = 0;
	}
}
