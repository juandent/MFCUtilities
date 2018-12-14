#pragma once
#include "CSVFile.h"

namespace Controller
{
	class LoadCompoundDocIntoDB
	{
		CSVFile m_file_reader;
	public:
		explicit LoadCompoundDocIntoDB(const std::string& file_name);
		~LoadCompoundDocIntoDB();
		void DoLoadIntoDB();
	};

}
