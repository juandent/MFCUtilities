#pragma once

#include <string>
#include <regex>
#include <locale>
#include <array>
#include <cassert>
// #include <fstream>
#include <sstream>
//#include <date.h>
#include <type_traits>
#include "StringDateConverter.h"
// #include <FixedPoint/Money.h>

#include <filesystem>

namespace fs = std::filesystem;

#ifndef DLLS_H
#define MFC_UTILITIES_EXPORTS
#include <Dlls.h>
#endif


class CGridCellBase;

#include "Buffer.h"

namespace JD {

	// Sample diagnostics class:
	namespace diag
	{
		template<typename T>
		struct TypeName;	// un purpose not defined!

#ifdef OUTPUT_TO_IMMEDIATE_WINDOW

		template<size_t Size>
		inline void display(const std::array<char, Size>& buffer)
		{
			std::string str;
			for (auto el : buffer)
			{
				if (el == '\0' || el == '\n')
					el = ' ';
				str += el;
			}
			str += "\n\0";
			::OutputDebugStringA(str.c_str());
		}
		inline void endl()
		{
			::OutputDebugStringA("\n");
		}
		inline void display(const std::wstring& str)
		{
			::OutputDebugStringW(str.c_str());
			endl();
		}
		inline void display(const std::string& str)
		{
			::OutputDebugStringA(str.c_str());
			endl();
		}
		inline void display(std::ifstream& in)
		{
			std::string str = "not_good == ";
			str += (!in.good()) ? "true" : "false";
			display(str);
		}
		inline void display(const std::vector<char>& buffer)
		{
			std::string str;
			for (auto el : buffer)
			{
				if (el == '\0' || el == '\n')
					el = ' ';
				str += el;
			}
			str += "\0";
			::OutputDebugStringA(str.c_str());
			endl();
		}
		inline void display(date::sys_days pt)
		{
			date::year_month_day ymd{ pt };
			std::ostringstream os;
			os << ymd;
			display(os.str());
		}

#else
		template<size_t Size>
		inline void display(const std::array<char, Size>& buffer)
		{}
		template<typename T>
		inline void display(const T&) {}
		inline void display(const std::vector<char>& buffer) {}
		inline void display(const std::string& str) {}
		inline void endl() {}
#endif
	}
	// using pointers
	template <typename T, typename U, typename X = std::remove_pointer<T>::type, typename Y = std::remove_pointer<U>::type,
		typename A = std::enable_if< std::is_base_of<Y, X>::value>::type,
		typename B = std::enable_if<std::is_pointer<T>::value>::type,
		typename C = std::enable_if<std::is_pointer<U>::value>::type
	>
		MFC_UTILITIES_API	T  polymorphic_cast(U p)
	{
		assert(dynamic_cast<T>(p));
		return static_cast<T>(p);
	}
	// using references
	template <typename T, typename U, typename X = std::remove_reference<T>::type, typename Y = std::remove_reference<U>::type,
		typename A = std::enable_if< std::is_base_of<Y, X>::value>::type>
		MFC_UTILITIES_API	T&  polymorphic_cast(U& p)
	{
		assert(dynamic_cast<T&>(p));
		return static_cast<T&>(p);
	}

	// convert string to uppercase
	inline std::string touppercase(const std::string& name)
	{
		// pass 'name' to upper case
		std::string upper_name{ name };
		std::transform(name.cbegin(), name.cend(), upper_name.begin(), [](char c) {
			return std::toupper(c, std::locale{});
		});
		return upper_name;
	}

	// convert string to lowercase
	inline std::string tolowercase(const std::string& name)
	{
		// pass 'name' to upper case
		std::string lower_name{ name };
		std::transform(name.cbegin(), name.cend(), lower_name.begin(), [](char c) {
			return std::tolower(c, std::locale{});
		});
		return lower_name;
	}



	// convert string to wstring
	inline std::wstring to_wstring(const std::string& str, const std::locale& loc = std::locale{})
	{
		std::vector<wchar_t> buf(str.size());
		std::use_facet<std::ctype<wchar_t>>(loc).widen(str.data(), str.data() + str.size(), buf.data());

		return std::wstring(buf.data(), buf.size());
	}

	// convert wstring to string
	inline std::string to_string(const std::wstring& str, const std::locale& loc = std::locale{})
	{
		std::vector<char> buf(str.size());
		std::use_facet<std::ctype<wchar_t>>(loc).narrow(str.data(), str.data() + str.size(), '?', buf.data());

		return std::string(buf.data(), buf.size());
	}

	inline CString to_cstring(const std::string& msg)
	{
		std::wstring message = to_wstring(msg);
		CString msg_as_cstring{ message.c_str() };
		return msg_as_cstring;
	}

	inline CString to_cstring( double d)
	{
		auto d_str = std::to_string(d);
		return to_cstring(d_str);
	}

	inline std::string to_string(date::sys_days dp)
	{
		date::year_month_day ymd{ dp };
		std::ostringstream os;
		os << ymd;
		return os.str();
	}

	inline CString to_cstring(date::sys_days dp)
	{
		auto str = to_string(dp);
		return to_cstring(str);
	}

	inline std::string from_cstring(const CString& msg)
	{
		auto m = static_cast<LPCTSTR>(msg);
		auto str = to_string(m);
		return str;
	}
	inline CString to_cstring(int val)
	{
		std::string str = std::to_string(val);
		return to_cstring(str);
	}
	inline CString to_cstring( bool val)
	{
		return CString{ val ? L"True" : L"False" };
	}

	inline CString to_cstring( Colones amount)
	{
		std::wstring tmp = static_cast<std::wstring>(amount);
		return CString{ tmp.c_str() };
	}

	inline CString to_cstring(Dolares amount)
	{
		std::wstring tmp = amount;
		return CString{ tmp.c_str() };
	}
#if 0
	template<class T>
	inline std::string to_string(T&& t)
	{
		std::ostringstream oss;
		oss << std::forward<T>(t);
		return oss.str();
	}

	template<>
	inline std::string to_string(std::wstring&& t)
	{
		to_string()
		std::ostringstream oss;
		oss << std::forward<T>(t);
		return oss.str();
	}
#endif
	template<typename Element>
	struct MFC_UTILITIES_API get_quote
	{
		static constexpr void const* chars[2] = {"\"", L"\"" };
		static constexpr Element const * quote_char = reinterpret_cast<Element const *>( chars[sizeof(Element) - 1]);
	};


	template<typename Element >
	inline std::basic_string<Element> quote(const std::basic_string<Element>& str)
	{
		constexpr Element const* quoted_char = get_quote<Element>::quote_char;

		std::basic_string<Element> quoted = quoted_char;
		quoted += str;
		quoted += quoted_char;
		return quoted;
	}

#if 0
	namespace boost_hana_namespace
	{
		using boost::hana::make_map;
		using boost::hana::make_pair;
		using boost::hana::type_c;

		constexpr auto m = make_map(
			make_pair(type_c<char>, '"'),
			make_pair(type_c<char16_t>, u'"'),
			make_pair(type_c<char32_t>, U'"'),
			make_pair(type_c<wchar_t>, L'"')
		);

		template<class CharT>
		std::basic_string<CharT> quote(const std::basic_string<CharT>& str)
		{
			return m[hana::type_c<CharT>] + str + m[hana::type_c<CharT>];
		}
	}
#endif
	struct SingletonsInitializer
	{
		SingletonsInitializer();
		~SingletonsInitializer();
	};

	extern SingletonsInitializer initializer;

	CString local_to_cstring(Money money, int width = 13);

	CString dollars_to_cstring(Money money, int width = 13);

	CString number_to_cstring(Money money, int width = 13);

	std::string to_string(Colones money, int width = 13);

	std::wstring to_wstring(Colones money, int width = 13);

	std::string to_string(Dolares money, int width = 13);

	std::wstring to_wstring(Dolares money, int width = 13);


	Money from_local_cstring(const CString& s);

	Money from_dollars_cstring(const CString& s);

	inline Money to_money(const std::string& s)
	{
		auto val = stold(s);
		return Money{ val };
	}

	inline long double strip_to_long_double(std::string moneyAsString)
	{
		std::string stripped;
		for (auto& c : moneyAsString)
		{
			if (c != '$' && c != '¢' && c != ',')
			{
				stripped += c;
			}
		}
		return stold(stripped);
	}

	inline Money strip_to_money(std::string moneyAsString)
	{
		auto val = strip_to_long_double(moneyAsString);
		return Money{ val };
	}

	inline COleDateTime to_ole_date_time(date::sys_days fecha)
	{
		using namespace date;
		
		year_month_day ymd = fecha;
		auto year_val = static_cast<int>(ymd.year());
		auto month_val = static_cast<unsigned>(ymd.month());
		auto day_val = static_cast<unsigned>(ymd.day());
		COleDateTime rDateTime;
		rDateTime.SetDate(year_val, month_val, day_val);
		return rDateTime;
	}

	inline date::sys_days to_sys_days(const COleDateTime& fecha)
	{
		using namespace date;
		
		int yearVal = fecha.GetYear();
		unsigned monthVal = fecha.GetMonth();
		unsigned dayVal = fecha.GetDay();

		year_month_day ymd{ year{yearVal}, month{monthVal}, day{dayVal} };
		sys_days date = ymd;
		return date;
	}

	inline std::string convert(const CString& str)
	{
		std::wstring s{ str };
		return to_string(s);
	}

	inline unsigned lineYear(int lineMonth, unsigned statementMonth, unsigned statementYear)
	{
		assert(statementYear != 0);

		if (lineMonth == statementMonth)
		{
			return statementYear;
		}
		else if (statementMonth == 1)
		{
			return statementYear - 1;
		}
		else
		{
			return statementYear;
		}
	}

	inline date::sys_days selected_criteria_to_date(const std::string& asText)
	{
		using namespace std;
		using namespace date;

		smatch m;

		regex regDigitsByDash{ "([[:d:]]{4})[-/]([[:d:]]{2})[-/]([[:d:]]{2})" };

		bool okSecondFormat = regex_search(asText, m, regDigitsByDash);

		if (okSecondFormat)
		{
			auto sDay = m.str(3);
			auto sMonth = m.str(2);
			auto sYear = m.str(1);

			unsigned intDay = stoi(sDay);
			unsigned intMonth = stoi(sMonth);
			unsigned intYear = stoi(sYear);

			year_month_day aDate{ year(intYear), month(intMonth), day(intDay) };
			return aDate;
		}
		return year_month_day{ year{0}, month{0}, day{0} };
	}

	inline date::sys_days to_date(const std::string& asText, unsigned statementMonth = 0, int statementYear = 0)
	{
		DateAsString::StringDateConverter converter{ asText, statementMonth, statementYear };
		auto ret = converter.convert();
		if (!ret.first)
		{
			throw std::logic_error("Cannot convert text to date");
		}
		return ret.second;
	}

	inline bool is_date(const std::string& asText)
	{
		DateAsString::StringDateConverter converter{ asText, 0, 0 };
		auto ret = converter.convert();
		return ret.first;
	}

	inline std::string remove_extension(const std::string& fileName)
	{
		//namespace fs = std::filesystem;

		auto x = fs::path(fileName);
		auto y = x.filename();
		auto file = Util::to_string(y);
		std::string::size_type n = file.find('.');
		// remove extension:
		if (n != std::string::npos)
		{
			file = file.substr(0, n);
		}
		return file;
	}

	inline CString weekDayCS(const date::sys_days dp)
	{
		static std::array<CString, 7> days{ L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday" };
		date::year_month_weekday ymwd{ dp };
		auto wkday = ymwd.weekday();
		int i = (wkday - date::Sunday).count();
		//auto i = ymwd.weekday_indexed().weekday().operator unsigned int();
		return days[i];
	}

	inline const std::string& weekDay(const date::sys_days dp)
	{
		static std::array<std::string, 7> days{ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
		date::year_month_weekday ymwd{ dp };
		auto wkday = ymwd.weekday();
		int i = (wkday - date::Sunday).count();
		//auto i = ymwd.weekday_indexed().weekday().operator unsigned int();
		return days[i];
	}

	inline LPARAM weekDayAsData(const date::sys_days dp)
	{
		auto& dayName = weekDay(dp);
		return reinterpret_cast<LPARAM>(dayName.c_str());
	}

	inline LPARAM stringAsData(const std::string& str)
	{
		return reinterpret_cast<LPARAM>(str.c_str());
	}
	///
	struct MFC_UTILITIES_API Comparison
	{
		static std::locale loc;
		static std::ostringstream os;

		static std::string stripNonDigits(const CString& s);

		inline static long double to_number(const std::string& s)
		{
			return stold(s);
		}
		inline static long double to_number(const CString& s)
		{
			auto str = stripNonDigits(s);
			return stold(str);
		}

		static std::pair<CGridCellBase*, CGridCellBase*> translate(LPARAM lpar1, LPARAM lpar2);
		static int __stdcall Text(LPARAM lpar1, LPARAM lpar2, LPARAM lpar3);
		static int __stdcall Money(LPARAM lpar1, LPARAM lpar2, LPARAM lpar3);
	};


	// for map or multimap insertion or updating: (Effective STL, pg 110 by Scott Meyers)
	template< typename MapType,
		typename KeyArgType,
		typename ValueArgType>
		auto MFC_UTILITIES_API efficientAddOrUpdate(MapType& m, const KeyArgType& k, const ValueArgType& v)
	{
		auto lb = m.lower_bound(k);			// find where k is or should be

		if (lb != m.end() && !(m.key_comp()(k, lb->first)))
		{
			lb->second = v;
			return lb;
		}
		else
		{
			using MVT = typename MapType::value_type;
			return m.insert(lb, MVT{ k,v });
		}
	}
	// utility for when pointers, iterators or smart pointers are stored in associative containers (Effective STL, pg 91, Scott Meyers)
	struct MFC_UTILITIES_API DereferenceLess
	{
		template<typename PtrType>
		bool operator()(PtrType pT1, PtrType pT2) const
		{
			return *pT1 < *pT2;
		}
	};

	using MoneyPair = std::pair<Money, Money>;
	inline MoneyPair& operator+=(MoneyPair& lhs, MoneyPair rhs)
	{
		lhs.first += rhs.first;
		lhs.second += rhs.second;
		return lhs;
	}

}
