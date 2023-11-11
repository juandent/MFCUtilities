#include "stdafx.h"
#include "GridDisplayer.h"

// #include "../Model/Model.ORM.Definitions.h"

// using namespace Model;

//void getPointerToDataMember( &Category::(*member))
//{
//	
//}


struct C
{
	const char* val;
	const char* val2;
	std::string str;
	std::string  str2;
	int i;
};





//template<typename T, typename ...DataType>
//DataType getDataPointer(T& z, DataType T::*p...)
//{
//	auto value = z.*p;
//	return value;
//}

template<typename T, typename DataType, typename DataType2>
DataType getDataPointer(T& z, DataType T::*p, DataType2 T::*q )
{
	auto value = z.*p;
	auto other = z.*q;
	return value;
}

template<typename T>
CString print(const T& t )
{
	return util::to_cstring(t);
}

template<typename T, typename ...Cols>
struct Q
{
	std::tuple<Cols...> tupleOfCols;
	Q(Cols...cls) : tupleOfCols(cls...)
	{}

	void getTupleItems()
	{
		constexpr const int size = std::tuple_size<tupleOfCols>;
		for (int i = 0; i < size; ++i)
		{
			auto element = std::tuple_element<i, tupleOfCols>;
		}
	}
};



template<typename T>  //, typename ...Cols>
struct P
{
	CJDGridCtrl grid;

	std::vector<C> lines{ C{"val", "val2", "str", "str2", 5}, C{"val2", "val3", "str2", "str3", 6 } };

	template<typename ...DataTypes>
	void display(DataTypes... dts)
	{
		for(int i=0; i < lines.size(); ++i)
		{
			printDataInGrid(i, 1, lines[i], dts...);
		}
	}

private:
	template<typename DataType>
	void printDataInGrid(int row, int col, T& z, DataType p)
	{
		auto value = z.*p;
		auto cs = print(value);
		grid.SetItemText(row + 1, col, cs);
	}


	template<typename DataType, typename ... RestDataTypes>
	void printDataInGrid(int row, int col, T& z, DataType p, RestDataTypes...qs)
	{
		auto value = z.*p;
		auto cs = print(value);
		grid.SetItemText(row + 1, col, cs);
		printDataInGrid(row, ++col, z, qs...);
	}

};


void doIT()
{
	using Pcchm = const char* C::*;
	using Pstring = std::string C::*;

	C z;
	C* p = &z;
	Pcchm pch = &C::val;
	Pstring pstr = &C::str;

	z.*pch = "hola";

	pch = &C::val2;
	z.*pch = "bye";
	z.*pstr = "hey!";


	getDataPointer(z, &C::val, &C::str);
	//getData(z, &C::val, &C::str);

	P<C> pp;

	pp.display(&C::val, &C::str);
	//pp.handle(&C::val, &C::str);
	
	//P<C, &C::val, &C::str> ppp;
	//ppp.callHandle();

	//Q<C, &C::val, &C::str> q();

	Q<C, decltype(&C::val), decltype(&C::str)> q(&C::val, &C::str);

	// Q<C> qq(&C::val, &C::str);


	CJDGridCtrl grid;
	std::vector<C> lines{ C{"val", "val2", "str", "str2", 5}, C{"val2", "val3", "str2", "str3", 6 } };
	std::vector<std::string> headers{ "NAME", "REAL?" };
	GridDisplayer<C> displayer( grid, std::move(lines), std::move( headers) );
	displayer.display(&C::val, &C::i, &C::str);

#if 0

	CJDGridCtrl grid;
	std::vector<Category> lines = ORM::storage.get_all<Category>();
	std::vector<std::string> headers{ "NAME", "REAL?" };



	GridDisplayer<Category, &Category::m_name_id, &Category::m_real_expense_or_income>
		displayer{ grid, lines, headers };
#endif

}
