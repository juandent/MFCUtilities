#include "stdafx.h"


#include "Controller.Interfaces.h"
#include "Controller.FileLoader.h"

void Controller::CategoryLoader::load(const std::string & file_name)
{
	m_file.load(file_name);
}

void Controller::CategoryLoader::store()
{
	for (int rcount = 1; rcount < m_file.getRowCount(); ++rcount)
	{
		auto& row = m_file.getRow(rcount);
		auto& col1 = row[1];
	}
}
