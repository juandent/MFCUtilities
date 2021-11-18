#pragma once

import CSV_File;

class PasswordLoader
{
	std::string m_fileName;
	csv::CSV_File m_csvFile;

	int getLocationId(std::string locationName);
	void createIfNotExist(std::string password, std::chrono::sys_days date, int location_id);
public:
	PasswordLoader(std::string fileName) : m_fileName{ fileName } {}
	void Load();
};

