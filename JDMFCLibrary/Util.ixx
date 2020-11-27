module;

#include <string>
#include <concepts>

export module Util;

using namespace std;


namespace Util
{
	export template<typename T> requires std::is_base_of<std::exception, T>::value
		std::string ProcessCodeException(const T& e)
	{
		using namespace std;

		auto c = e.code();

		ostringstream os;

		os << c.category().name() << " " << c.value() << " " << c.message() << " ";

		return os.str();
	}
}
