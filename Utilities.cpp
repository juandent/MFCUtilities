#include "stdafx.h"
#include "Utilities.h"
#include <algorithm>
#include <array>
#include <locale>
#include <iomanip>
#include "GridCellBase.h"



using namespace std;

namespace JD
{
//#define USING_HANA
#ifdef USING_HANA

	struct boost_hana
	{
	private:
		static constexpr auto m = boost::hana::make_map(
			boost::hana::make_pair(boost::hana::type_c<char>, '"'),
			boost::hana::make_pair(boost::hana::type_c<char16_t>, u'"'),
			boost::hana::make_pair(boost::hana::type_c<char32_t>, U'"'),
			boost::hana::make_pair(boost::hana::type_c<wchar_t>, L'"')
		);
	public:
		template<class CharT>
		static std::basic_string<CharT> quote(const std::basic_string<CharT>& str)
		{
			return m[boost::hana::type_c<CharT>] + str + m[boost::hana::type_c<CharT>];
		}
	};

	using namespace boost;
	constexpr auto m = hana::make_map(
		hana::make_pair(hana::type_c<char>, '"'),
		hana::make_pair(hana::type_c<char16_t>, u'"'),
		hana::make_pair(hana::type_c<char32_t>, U'"'),
		hana::make_pair(hana::type_c<wchar_t>, L'"')
	);

	template<class CharT>
	std::basic_string<CharT> quote(const std::basic_string<CharT>& str)
	{
		return m[hana::type_c<CharT>] + str + m[hana::type_c<CharT>];
	}
#endif

#if 0
	array<string,12> MonthNames::monthNames{ "ENE", "FEB", "MAR", "ABR", "MAY", "JUN", "JUL", "AGO", "SEP", "OCT", "NOV", "DIC" };

	unsigned MonthNames::getMonthNumber(const string& name)
	{
		// pass 'name' to upper case
		string upper_name{ name };
		transform(name.cbegin(), name.cend(), upper_name.begin(), [](char c) {
			return std::toupper(c, std::locale{});
		});

		auto pos = find_if( monthNames.cbegin(), monthNames.cend(), [upper_name= upper_name](const string& monthName) 
			{
			return upper_name == monthName;
		});
		return distance(monthNames.cbegin(), pos) + 1;
	}
#endif

	// to deal with unneeded positive sign!!
	class costaRicaNumPunct : public std::moneypunct_byname<char>
	{
	public:
		explicit costaRicaNumPunct(const std::string& name)
			: std::moneypunct_byname<char>("en-US")      ///   "sp-CR")
		{}
	protected:
		virtual std::string do_positive_sign() const override {
			return "";
		}
		virtual std::string do_curr_symbol() const override {
			return "¢";
		}
		virtual int do_frac_digits() const override
		{
			return 2;
		}
		virtual char_type do_thousands_sep() const override {
			return ',';
		}
		virtual char_type do_decimal_point() const override {
			return '.';
		}
	};


	string Comparison::stripNonDigits(const CString& s)
	{
		os.str("");
		for (int i = 0; i < s.GetLength(); ++i)
		{
			auto c = s.GetAt(i);
			if (c == '-' || c == '.' || std::isdigit(c, loc))
			{
				os << static_cast<char>(c);
			}
		}
		return os.str();
	}

	int __stdcall Comparison::Text(LPARAM lpar1, LPARAM lpar2, LPARAM lpar3)
	{
		auto cells = translate(lpar1, lpar2);
		if (cells.first == nullptr)  return 0;
		return _tcscmp(cells.first->GetText(), cells.second->GetText());
	}

	int __stdcall Comparison::Money(LPARAM lpar1, LPARAM lpar2, LPARAM lpar3)
	{
		auto cells = translate(lpar1, lpar2);
		if (cells.first == nullptr)  return 0;

		auto nValue1 = to_number(cells.first->GetText());
		auto nValue2 = to_number(cells.second->GetText());

		if (nValue1 < nValue2)
			return -1;
		else if (nValue1 == nValue2)
			return 0;
		else
			return 1;
	}

	pair<CGridCellBase*, CGridCellBase*> Comparison::translate(LPARAM lpar1, LPARAM lpar2)
	{
		CGridCellBase* pCell1 = reinterpret_cast<CGridCellBase*>(lpar1);
		CGridCellBase* pCell2 = reinterpret_cast<CGridCellBase*>(lpar2);
		if (!pCell1 || !pCell2) return make_pair(nullptr, nullptr);
		else return make_pair(pCell1, pCell2);
	}


	locale Comparison::loc;
	ostringstream Comparison::os;

#define PLAYING_WITH_LOCALES

	struct MoneytaryHelper
	{
	private:
		costaRicaNumPunct* m_posSignEliminator;
		locale m_usLoc;
		locale m_crLoc;
		ostringstream m_usOs;
		ostringstream m_crOs;
		istringstream m_usIs;
		istringstream m_crIs;

		const money_put<char>& m_usMoney_put;
		const money_put<char>& m_crMoney_put;

		const num_put<char>&   m_usNumber_put;
		const num_put<char>&   m_crNumber_put;
	public:
		MoneytaryHelper()
			: m_posSignEliminator{ new costaRicaNumPunct{ "" } },
#ifndef PLAYING_WITH_LOCALES
			m_usLoc{ "en-US" }, m_crLoc{ locale{"sp-ES"}.combine<numpunct<char>>(locale{"sp-CR"} ), //, m_posSignEliminator
	},
#else
			m_usLoc{ "en-US" }, m_crLoc{ m_usLoc, m_posSignEliminator },
#endif
			m_usMoney_put{ use_facet<money_put<char>>(m_usLoc) }, m_crMoney_put{ use_facet<money_put<char>>(m_crLoc)},
			m_usNumber_put { use_facet<num_put<char>>(m_usLoc) }, m_crNumber_put{ use_facet<num_put<char>>(m_crLoc) }
		{
			m_usOs << showbase;
			m_usOs.imbue(m_usLoc);

			m_crOs << showbase;
			m_crOs.imbue(m_crLoc);

			m_usIs.imbue(m_usLoc);
			m_crIs.imbue(m_crLoc);


#ifdef PLAYING_WITH_LOCALES
			//m_crLoc = m_crLoc.combine<numpunct<char>>(locale{"sp-ES"});
			//m_crOs.imbue(m_crLoc);
			//m_crIs.imbue(m_crLoc);
#endif
		}
		string putNumberAsCR(Money money, int width)
		{
			m_crOs.str("");
			m_crOs << right << setw(width);
			auto value = money.rep() / (long double)money.scale_;
			m_crNumber_put.put(m_crOs, m_crIs, ' ', value);
			auto temp = m_crOs.str();
			return temp;
		}
		string putColones(Money money, int width)
		{
			m_crOs.str("");
			m_crOs << right << setw(width);
			m_crMoney_put.put(m_crOs, false, m_crOs, ' ', money.getAsLongDouble()); //   money.rep());
			auto temp = m_crOs.str();
			return temp;
		}
		string putDollars(Money money, int width)
		{
			m_usOs.str("");
			m_usOs << right << setw(width);
			m_usMoney_put.put(m_usOs, false, m_usOs, ' ', money.getAsLongDouble());	// money.rep());
			auto temp = m_usOs.str();
			return temp;
		}
		Money getColones(const std::string& asColones)
		{
			m_crIs.str(asColones);
			auto readVal = 0.0L;
			m_crIs >> get_money(readVal);
			return Money(readVal);
		}
		Money getDollars(const std::string& asDollars)
		{
			m_crIs.str(asDollars);
			auto readVal = 0.0L;
			m_crIs >> get_money(readVal);
			return Money(readVal);
		}
	};

	namespace
	{
		int s_NiftyCounterForSingletons = 0;
		MoneytaryHelper* moneyHelper = nullptr;
		SingletonsInitializer initializer{};
	}


	SingletonsInitializer::SingletonsInitializer()
	{
		if (s_NiftyCounterForSingletons++ == 0)
		{
			moneyHelper = new MoneytaryHelper{};
		}
	}

	SingletonsInitializer::~SingletonsInitializer()
	{
		if (--s_NiftyCounterForSingletons == 0)
		{
			delete moneyHelper;
		}
	}

	CString number_to_cstring(Money money, int width)
	{
		auto temp = moneyHelper->putNumberAsCR(money, width);
		return to_cstring(temp);
	}

	CString local_to_cstring(Money money, int width)
	{
		auto temp = moneyHelper->putColones(money, width);
		return to_cstring(temp);
	}
	
	CString dollars_to_cstring(Money money, int width)
	{
		auto temp = moneyHelper->putDollars(money, width);
		return to_cstring(temp);
	}

	///
	std::string to_string(Colones money, int width)
	{
		auto temp = moneyHelper->putColones(money, width);
		return temp;
	}

	std::wstring to_wstring(Colones money, int width)
	{
		auto temp = to_string(money, width);
		return to_wstring(temp);
	}

	std::string to_string(Dolares money, int width )
	{
		auto temp = moneyHelper->putDollars(money, width);
		return temp;
	}

	std::wstring to_wstring(Dolares money, int width )
	{
		auto temp = to_string(money, width);
		return to_wstring(temp);
	}







	Money from_local_cstring(const CString & s)
	{
		auto str = from_cstring(s);
		auto temp = moneyHelper->getColones(str);
		return temp;
	}

	Money from_dollars_cstring(const CString & s)
	{
		auto str = from_cstring(s);
		auto temp = moneyHelper->getDollars(str);
		return temp;
	}

};

