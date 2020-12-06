// Catch_Testing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <catch2/catch.hpp>
//#include <catch2/catch_reporter_automake.hpp>
//#include <catch2/catch_reporter_sonarqube.hpp>
// #include <catch2/catch_reporter_tap.hpp>
// #include <catch2/catch_reporter_teamcity.hpp>


TEST_CASE("Learning Catch")
{
	REQUIRE(1 == 1);
	REQUIRE(1 != 2);

	SECTION("TRY addition")
	{
		int i = 7;
		REQUIRE(++i == 8);
		std::cout << "Hola"<< std::endl;
	}
	
}

#if 0
int main()
{
    std::cout << "Hello World!\n";
}
#endif

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
