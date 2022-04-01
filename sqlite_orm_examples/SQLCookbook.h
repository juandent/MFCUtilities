#pragma once

#include <sqlite_orm/sqlite_orm.h>
#include <optional>
#include <string>

#include "SQLCookbook.h"

struct Employee
{
	int m_empno;
	std::string m_ename;
	std::string m_job;
	std::optional<int> m_mgr;
	std::string m_hiredate;
	double m_salary;
	std::optional<double> m_commission;
	int m_deptno;
};

struct Department
{
	int m_deptno;
	std::string m_deptname;
	std::string m_loc;

};

struct EmpBonus
{
	int m_id;
	int m_empno;
	std::string m_received;	// date
	int m_type;
};

struct Artist
{
	int m_id;
	std::string m_name;
};

struct Album
{
	int m_id;
	int m_artist_id;
};

using namespace sqlite_orm;

auto storage = make_storage("SQLCookbook.sqlite",
	make_table("Emp",
		make_column("empno", &Employee::m_empno, primary_key(), autoincrement()),
		make_column("ename", &Employee::m_ename),
		make_column("job", &Employee::m_job),
		make_column("mgr", &Employee::m_mgr),
		make_column("hiredate", &Employee::m_hiredate),
		make_column("salary", &Employee::m_salary),
		make_column("comm", &Employee::m_commission),
		make_column("deptno", &Employee::m_deptno),
		foreign_key(&Employee::m_deptno).references(&Department::m_deptno)),
	make_table("Dept",
		make_column("deptno", &Department::m_deptno, primary_key(), autoincrement()),
		make_column("deptname", &Department::m_deptname),
		make_column("loc", &Department::m_loc)),
	make_table("Emp_bonus",
		make_column("id", &EmpBonus::m_id, primary_key(), autoincrement()),
		make_column("empno", &EmpBonus::m_empno),
		make_column("received", &EmpBonus::m_received),
		make_column("type", &EmpBonus::m_type),
		foreign_key(&EmpBonus::m_empno).references(&Employee::m_empno)),
	make_table("Artists",
		make_column("id", &Artist::m_id, primary_key(), autoincrement()),
		make_column("name", &Artist::m_name)),
	make_table("Albums",
		make_column("id", &Album::m_id, primary_key(), autoincrement()),
		make_column("artist_id", &Album::m_artist_id),
		foreign_key(&Album::m_artist_id).references(&Artist::m_id)));


/*
 * Employee -> Department
 * EmpBonus -> Employee
 * Album -> Artist
 *
 *
 *	INSERT ORDER:
 *
 * 1- Department
 * 2- Employee
 * 3- EmpBonus
 * 4- Artist
 * 5- Album
 *
 *	DROP ORDER
 *
 *	1- Album
 *	2- Artist
 *	3- EmpBonus
 *	4- Employee
 *	5- Department
 *
 */

#include <Typelist.h>	// loki

#if 0
namespace Loki
{
	namespace TL
	{
		template <class TList, class T> struct MostDerived;

		template <class T>
		struct MostDerived<NullType, T>
		{
			typedef T Result;
		};

		template <class Head, class Tail, class T>
		struct MostDerived<Typelist<Head, Tail>, T>
		{
		private:
			typedef typename MostDerived<Tail, T>::Result Candidate;
		public:
			typedef typename Select<
				SUPERSUBCLASS(Candidate, Head),
				Head, Candidate>::Result Result;
		};
	}
}
#endif


template<typename ForeignKeyTable, typename ...PKTables>
struct FKDependency
{
	using lesser = ForeignKeyTable;
	static std::tuple<PKTables...> pk_list;
};


template<typename TableA, typename TableB, typename FKDep>
constexpr bool isDefined()	// is TableA -> TableB defined??
{
}


template<typename TableB, typename Tuple, size_t N>
struct is_found
{
	using Head = std::tuple_element<N - 1, Tuple>::type;

	static constexpr bool value = std::is_same_v<TableB, Head> || is_found<TableB, Tuple, N-1>::value;
};

template<typename TableB, typename Tuple>
struct is_found<TableB, Tuple, 1>
{
	using Head = std::tuple_element<0, Tuple>::type;

	static constexpr bool value = std::is_same_v<TableB, Head>;
};



template<typename TableA, typename TableB, typename FKDep>
struct IsLessThan
{
	using pk_list = typename FKDep::pk_list;
	static constexpr size_t size_of_pk_list = std::tuple_size_v<pk_list>;

	static_assert(std::is_same_v<TableA, FKDep::lesser>,"TableA not equal to FKDependency::lesser");

	static constexpr bool value = is_found<TableB, pk_list, size_of_pk_list >::value;
};

template<typename FKTable, typename PKTable, typename... AllTables >
struct Order
{
	using I = FKTable;
	using D = PKTable;


};
#if 0
namespace errors
{
	template<typename T, size_t Pos, size_t Size, typename...Tables>
	struct find_in_tuple;

	template<typename T, size_t Size, typename Head>
	struct find_in_tuple<T, Size, Size, Head>
	{
		static constexpr int value = std::is_same<T, Head>::value ? Size - 1 : -1;
	};

	template<typename T, size_t Pos, size_t Size, typename Head, typename...Tail>
	struct find_in_tuple
	{
		static constexpr int value = std::is_same<T, Head>::value ? Pos : find_in_tuple<T, Pos + 1, Size, Tail...>::value;
	};



	template<typename I, typename D, typename ...Tables>
	struct tuple_divide
	{
		static constexpr std::tuple<Tables...> table_list;
		static constexpr size_t size = sizeof...(Tables);

		static constexpr size_t pos_i = find_in_tuple<I, 0, size, Tables...>::value;
	};
}
#endif

template<typename I, int Pos, int Size, typename Tuple>
struct find_in_tuple;

template<typename I, int Size, typename Tuple>
struct find_in_tuple<I, Size , Size, Tuple>
{
	static constexpr int pos = -1;
};


template<typename I, int Pos, int Size, typename Tuple>
struct find_in_tuple
{
	using type = std::tuple_element<Pos, Tuple>::type;
	static constexpr int pos = std::is_same<type, I>::value ? Pos : find_in_tuple<I, Pos + 1, Size, Tuple>::pos;
};


template<typename I, typename D, typename Tuple>
struct tuple_divide
{
	constexpr static void split( Tuple t)
	{
		
	}
	static constexpr size_t size = std::tuple_size_v<Tuple>;

	static constexpr int pos_i = find_in_tuple<I, 0, size, Tuple>::pos;
	static constexpr int pos_d = find_in_tuple<D, 0, size, Tuple>::pos;

};


// *1 - Department
// * 2 - Employee
// * 3 - EmpBonus
// * 4 - Artist
// * 5 - Album

using ListOfTables = std::tuple< Album, Artist, EmpBonus, Employee, Department>;
constexpr int pos_i = tuple_divide<Employee, Department, ListOfTables>::pos_i;
constexpr int pos_d = tuple_divide<Employee, Department, ListOfTables>::pos_d;

template<typename...Tables>
struct List
{
	static std::tuple<Tables...> basic_list;

	template<typename FKDep, typename ListOfTables>
	struct OrderBy
	{

	};
};

//
template < size_t count, std::size_t... Ns, typename... Ts>
auto head_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
{
	return  std::make_tuple(std::get<Ns>(t)...);
}



template < size_t count, typename T, typename... Ts >
auto head(std::tuple<T, Ts...> t)
{
	return head_impl<count>(std::make_index_sequence<count>(), t);
	// return  std::get<0>(t);
}

template < size_t offset, std::size_t... Ns, typename... Ts>
auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
{
	return  std::make_tuple(std::get<Ns + offset>(t)...);
}

template < size_t offset, typename... Ts>
auto tail(std::tuple<Ts...> t)
{
	return  tail_impl<offset>(std::make_index_sequence<sizeof...(Ts) - offset>(), t);
}

/*
*1 - Album
* 2 - Artist
* 3 - EmpBonus
* 4 - Employee
* 5 - Department
*/



void useTuples()
{
	tuple_divide<Employee, Department, ListOfTables>::split(ListOfTables{});

	// auto ttt = std::make_tuple < Album, Artist, EmpBonus, Employee, Department> ();
	auto tt = std::make_tuple(Album{}, Artist{}, EmpBonus{}, Employee{}, Department{});
	using ttType = decltype(tt);
	constexpr int pos_i = tuple_divide<Employee, Department, ttType>::pos_i;
	//auto t = std::make_tuple(2, 3.14, 'c');
	// auto h = head(tt);
	auto hh = head<pos_i>(tt);
	auto tl = tail<pos_i>(tt);
	// auto h0 = std::get<0>(hh);
	// auto h1 = std::get<1>(hh);
	auto t0 = std::get<0>(tail<2>(tt));
	using t0T = decltype(t0);
	constexpr bool same = std::is_same<t0T, EmpBonus>::value;
	auto t1 = std::get<1>(tail<2>(tt));
	using t1T = decltype(t1);
}




