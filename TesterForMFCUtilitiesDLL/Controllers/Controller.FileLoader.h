#pragma once

#include "Controller.Main.h"
#include "Controller.Interfaces.h"
#include "CSVFile.h"

namespace Controller
{

	class CategoryLoader : public  ICategoryLoader
	{
		CSVFile m_file;
	public:
		void load(const std::string& file) override;
		void store() override;
	};

}
