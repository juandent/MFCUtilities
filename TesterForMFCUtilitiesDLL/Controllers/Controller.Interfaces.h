#pragma once

#include <string>

namespace Controller
{
	struct ICategoryLoader
	{
		virtual void load(const std::string& file) = 0;
		virtual void store() = 0;
	};

}