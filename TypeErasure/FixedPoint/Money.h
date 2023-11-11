#pragma once

#include <FixedPoint/arithmetic_types.h>
#include <string>
#include <sstream>


#ifndef DLLS_H
#define MFC_UTILITIES_EXPORTS
#include <Dlls.h>
#endif


using Money = fixed_decimal<2, long long>;


// MFC_UTILITIES_API

class  Colones : public Money
{
	unsigned m_width = 13;
public:
	using Money::Money;
	Colones() : Money(0) {}
	Colones( const Money& money) 
		: Money(money.rep()/ (long double)money.scale_)
	{}
	void setWidth(unsigned w) { m_width = w; }
	operator std::string();
	operator std::wstring();
};

// MFC_UTILITIES_API
class  Dolares : public Money
{
	unsigned m_width = 13;
public:
	using Money::Money;
	Dolares() : Money(0) {}
	Dolares(const Money& money)
		: Money(money.rep() / (long double)money.scale_)
	{}
	void setWidth(unsigned w) { m_width = w; }
	operator std::string();
	operator std::wstring();
};

#if 0
#ifdef _DEBUG
inline void  testMoneySubclasses()
{
	Colones c{ 34.55 };
	auto cc = c.rep();
	Dolares d{ 47.99 };
	auto dd = d.rep();
}
#endif
#endif
