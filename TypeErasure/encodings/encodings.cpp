// encodings.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <locale>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

wstring str_wstr(const string& str)
{
	wstring ws;
	for (char c : str)
	{
		ws += c;
	}
	return ws;
}


int main()
{
	auto ln = std::locale::locale();
	auto nnn = ln.name();
	std::string name = u8"Muñoz";
	auto ene = 'ñ';
	signed char enne{ 'ñ' };
	unsigned short s_enne = enne;
	const double number = 1000.25;
	std::wostringstream oss;
	oss << "\"C\" locale: " << number << '\n';
	std::locale loc = std::locale("es_CR.UTF8");
	std::locale lmixed = loc.combine<std::moneypunct<char>>(std::locale("en_US.UTF8"));
	oss.imbue(lmixed);
	oss << "\"C\" locale with es_CR: " << enne << "  " << str_wstr(loc.name()) << " " << std::put_money(number) << '\n';
	std::wstring oss_str = oss.str();

    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
