module;

#include <iostream>
#include <algorithm>


export module StringLib;

export void MyFunc();

export namespace string_lib
{
    using namespace std;

    bool compareStringsNoCase(string first, string second) {
        transform<decltype(first.begin()), decltype(first.begin()), int(*)(int) >(first.begin(), first.end(), first.begin(), std::tolower);
        transform<decltype(first.begin()), decltype(first.begin()), int(*)(int) >(second.begin(), second.end(), second.begin(), std::tolower);
        //transform(first.begin(), first.end(), first.begin(), std::tolower);
        //transform(second.begin(), second.end(), second.begin(), std::tolower);

        return first == second;
    }


}

