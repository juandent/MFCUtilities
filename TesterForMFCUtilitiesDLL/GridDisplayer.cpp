#include "stdafx.h"
//#include "GridDisplayer.h"

#include "../Model/Model.ORM.Definitions.h"

using namespace Model;

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
	return JD::to_cstring(t);
}

#if 0
template<typename T, typename DataType>
void getData(T& z, DataType p)
{
	auto value = z.*p;
	auto cs = print(value);
}


template<typename T, typename DataType, typename ... RestDataTypes>
void getData(T& z, DataType p, RestDataTypes...qs)
{
	auto value = z.*p;
	auto cs = print(value);
	getData(z, qs...);
}
#endif


template<typename T>  //, typename ...Cols>
struct P
{
	CJDGridCtrl grid;

	std::vector<C> lines{ C{"val", "val2", "str", "str2", 5}, C{"val2", "val3", "str2", "str3", 6 } };

#if 0
	//void callHandle()
	//{
	//	doHandle(Cols...cls);
	//}

	//void doHandle(Cols... dts)
	//{
	//	getData(lines[0], dts...);
	//	getData(lines[1], dts...);
	//}

	template<typename ...DataTypes>
	void handle(DataTypes... dts)
	{
		getData(lines[0], dts...);
		getData(lines[1], dts...);
	}
#endif
	template<typename ...DataTypes>
	void display(DataTypes... dts)
	{
		getData(0, 1, lines[0], dts...);
		getData(1, 1, lines[1], dts...);
	}

	template<typename DataType>
	void getData(int row, int col, T& z, DataType p)
	{
		auto value = z.*p;
		auto cs = print(value);
		grid.SetItemText(row + 1, col, cs);
	}


	template<typename DataType, typename ... RestDataTypes>
	void getData(int row, int col, T& z, DataType p, RestDataTypes...qs)
	{
		auto value = z.*p;
		auto cs = print(value);
		grid.SetItemText(row + 1, col, cs);
		getData(row, ++col, z, qs...);
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

#if 0

	CJDGridCtrl grid;
	std::vector<Category> lines = ORM::storage.get_all<Category>();
	std::vector<std::string> headers{ "NAME", "REAL?" };



	GridDisplayer<Category, &Category::m_name_id, &Category::m_real_expense_or_income>
		displayer{ grid, lines, headers };
#endif

}
