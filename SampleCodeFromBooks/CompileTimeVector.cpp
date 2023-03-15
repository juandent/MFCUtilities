#include "CompileTimeVector.h"


#include <iostream>
#include <array>

#if 0
int main()
{
	constexpr std::array orig{ 0, 8, 15, 132, 4, 77 };

	constexpr auto avg = modifiedAvg(orig);

	constexpr auto merged = mergeValues(orig, 42, 4);

	for(auto& el : merged)
	{
		std::cout << el << std::endl;
	}
//	std::cout << "average: " << avg << '\n';
	int i = 0;
}
#endif


#include <iostream>
#include <string>
#include <array>
#include <cassert>

#include <string>
#include <format>

// convert an integral value to a std::string
// - can be called at compile time or runtime
//constexpr
consteval 
std::string asString(long long value)
{
	if (std::is_constant_evaluated()) {
		// compile-time version:
		if (value == 0) {
			return "0";
		}
		if (value < 0) {
			return "-" + asString(-value);
		}
		std::string s = asString(value / 10) + std::string(1, value % 10 + '0');
		if (s.size() > 1 && s[0] == '0') {  // skip leading 0 if there is one
			s.erase(0, 1);
		}
		return s;
	}
	else {
		// runtime version:
		return std::format("{}", value);
	}
}

// function template to export a compile-time string to runtime:
template<int MaxSize>
consteval auto toRuntimeString(std::string s)
{
	// ensure the size of the exported array is large enough:
	assert(s.size() <= MaxSize);

	// create a compile-time array and copy all characters into it: 
	std::array<char, MaxSize + 1> arr{};  // ensure all elems are initialized
	for (int i = 0; i < s.size(); ++i) {
		arr[i] = s[i];
	}

	// return the compile-time array and the string size:
	return std::pair{ arr, s.size() };
}

// function to import an exported compile-time string at runtime:
consteval std::string fromComptimeString(const auto& dataAndSize)
{
	// init string with exported array of chars and size:
	return std::string{ dataAndSize.first.data(),
					   dataAndSize.second };
}

// test the functions:
consteval
auto comptimeMaxStr()
{
	std::string s = "max int is " + asString(std::numeric_limits<int>::max())
		+ " (" + asString(std::numeric_limits<int>::digits + 1)
		+ " bits)";

	return toRuntimeString<100>(s);
}

consteval
void callCompTimeFromHere()
{
	auto ss = asString(2000);
	auto x = ss.size();
	std::string s = fromComptimeString(comptimeMaxStr());

}

int main()
{
	std::string s;// = fromComptimeString(comptimeMaxStr());
	callCompTimeFromHere();
	std::cout << s << '\n';
}



