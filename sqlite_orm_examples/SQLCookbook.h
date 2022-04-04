#pragma once

#include <sqlite_orm/sqlite_orm.h>
#include <optional>
#include <string>

#include "..\ORM_Extensions/SchemaManager.h"
#include "..\ORM_Extensions/update_schema.h"

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
	std::optional<int> m_manager;
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
		foreign_key(&Employee::m_deptno).references(&Department::m_deptno),
		check(c(&Employee::m_salary) > &Employee::m_commission)),
	make_table("Dept",
		make_column("deptno", &Department::m_deptno, primary_key(), autoincrement()),
		make_column("deptname", &Department::m_deptname),
		make_column("loc", &Department::m_loc),
		make_column("mgr", &Department::m_manager),
		foreign_key(&Department::m_manager).references(&Employee::m_empno)),
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

#endif

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

#ifdef ORIGINAL_CODE
namespace TableOrder
{

	template<typename I, int Pos, int Size, typename Tuple>
	struct find_in_tuple;

	template<typename I, int Size, typename Tuple>
	struct find_in_tuple<I, Size, Size, Tuple>
	{
		static constexpr int pos = -1;
	};


	template<typename I, int Pos, int Size, typename Tuple>
	struct find_in_tuple
	{
		using type = std::tuple_element<Pos, Tuple>::type;
		static constexpr int pos = std::is_same<type, I>::value ? Pos : find_in_tuple<I, Pos + 1, Size, Tuple>::pos;
	};

	// for reversing tuples:
	// https://stackoverflow.com/questions/17178075/how-do-i-reverse-the-order-of-element-types-in-a-tuple-type#:~:text=Or%20you%20can%20write%20a%20function%20to%20reverse,type.%20To%20reverse%20a%20tuple-like%20object%20in%20C%2B%2B14%3A

	template <typename... Ts>
	struct tuple_reverse;

	template <>
	struct tuple_reverse<std::tuple<>>
	{
		using type = std::tuple<>;
	};

	template <typename T, typename... Ts>
	struct tuple_reverse<std::tuple<T, Ts...>>
	{
		using head = std::tuple<T>;
		using tail = typename tuple_reverse<std::tuple<Ts...>>::type;

		using type = decltype(std::tuple_cat(std::declval<tail>(), std::declval<head>()));
	};

	template<typename I, typename D, typename Tuple>
	struct tuple_divide
	{
	private:
		static constexpr size_t size = std::tuple_size_v<Tuple>;

		static constexpr int pos_i = find_in_tuple<I, 0, size, Tuple>::pos;
		static constexpr int pos_d = find_in_tuple<D, 0, size, Tuple>::pos;

		template < size_t count, std::size_t... Ns, typename... Ts>
		static constexpr auto head_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
		{
			return  std::make_tuple(std::get<Ns>(t)...);
		}
		template < size_t count, typename T, typename... Ts >
		static constexpr auto head(std::tuple<T, Ts...> t)
		{
			return head_impl<count>(std::make_index_sequence<count>(), t);
		}
		template < size_t offset, std::size_t... Ns, typename... Ts>
		static constexpr auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
		{
			return  std::make_tuple(std::get<Ns + offset>(t)...);
		}

		template < size_t offset, typename... Ts>
		static constexpr auto tail(std::tuple<Ts...> t)
		{
			return  tail_impl<offset>(std::make_index_sequence<sizeof...(Ts) - offset>(), t);
		}
	public:
		constexpr static auto convert()
		{
			Tuple t{};

			if constexpr (pos_i > pos_d)						// relative position of Tables is incorrect
			{
				auto t3 = tail<pos_i + 1>(t);					// get tail after i

				auto t2plus = tail<pos_d + 1>(t);				// get tail after d

				auto t2 = head<pos_i - pos_d - 1>(t2plus);		// get elements between d and i

				auto t1 = head<pos_d>(t);						// get elements before d

				auto pair = std::tuple<I, D>();					// create the insertion tuple

				auto mix = std::tuple_cat(t1, pair, t2, t3);	// concatenate tuples into final mix

				return mix;
			}
			else												// no changes needed, return Tuple instance
			{
				return t;
			}
		}

	};
}
#endif
// *1 - Department
// * 2 - Employee
// * 3 - EmpBonus
// * 4 - Artist
// * 5 - Album



// template<typename...Tables>
// struct List
// {
// 	static std::tuple<Tables...> basic_list;
//
// 	template<typename FKDep, typename ListOfTables>
// 	struct OrderBy
// 	{
//
// 	};
// };
#if 0
//
template < size_t count, std::size_t... Ns, typename... Ts>
constexpr auto head_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
{
	return  std::make_tuple(std::get<Ns>(t)...);
}



template < size_t count, typename T, typename... Ts >
constexpr auto head(std::tuple<T, Ts...> t)
{
	return head_impl<count>(std::make_index_sequence<count>(), t);
	// return  std::get<0>(t);
}

template < size_t offset, std::size_t... Ns, typename... Ts>
constexpr auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t)
{
	return  std::make_tuple(std::get<Ns + offset>(t)...);
}

template < size_t offset, typename... Ts>
constexpr auto tail(std::tuple<Ts...> t)
{
	return  tail_impl<offset>(std::make_index_sequence<sizeof...(Ts) - offset>(), t);
}
#endif
/*
*1 - Album
* 2 - Artist
* 3 - EmpBonus
* 4 - Employee
* 5 - Department
*/

// *Employee->Department
// * EmpBonus->Employee
// * Album->Artist
#if 0
namespace TableOrder
{
	template<typename Tuple>
	inline constexpr auto getDropOrder()
	{
		auto newTuple = tuple_divide<Employee, Department, Tuple>::convert();
		auto newerTuple = tuple_divide<EmpBonus, Employee, decltype(newTuple)>::convert();
		static auto newestTuple = tuple_divide<Album, Artist, decltype(newerTuple)>::convert();
		return newestTuple;
	}

	template<typename Tuple>
	inline constexpr auto getInsertOrder()
	{
		return tuple_reverse<Tuple>::type();
	}

	using ListOfTables = std::tuple< Artist, Department, Album, Employee, EmpBonus>;

	inline constexpr size_t TableCount = std::tuple_size_v<ListOfTables>;

	template<size_t Pos, typename Storage>
	inline constexpr auto drop_table(Storage storage)
	{
		auto drop_order = getDropOrder<ListOfTables>();
		using Element = std::tuple_element_t<Pos, decltype(drop_order)>;
		std::vector<Element> vec;
		vec = storage.get_all<Element>();
		storage.drop_table(storage.tablename<Element>());
		return vec;
	}

	template<typename Storage, typename Vector>
	inline constexpr void replace_table(Storage storage, const Vector& vec )
	{
		storage.replace_range(vec.begin(), vec.end());
	}
}
#endif

#ifdef ORIGINAL_CODE
template<typename ConcreteDatabase>
struct SchemaManager
{
	SchemaManager()
	{
	}
	auto getDropOrder() const noexcept
	{
		return static_cast<const ConcreteDatabase*>(this)->getDropOrderImpl();
	}
	auto getInsertOrder() const noexcept
	{
		auto dropOrder = getDropOrder();
		return TableOrder::tuple_reverse<decltype(dropOrder)>::type();
	}
	template<size_t Pos, typename Storage>
	auto drop_table(Storage storage)
	{
		auto drop_order = getDropOrder();
		using Element = std::tuple_element_t<Pos, decltype(drop_order)>;
		std::vector<Element> vec;
		vec = storage.get_all<Element>();
		storage.drop_table(storage.tablename<Element>());
		return vec;
	}
	template<typename Storage, typename Vector>
	void replace_table(Storage storage, const Vector& vec)
	{
		storage.replace_range(vec.begin(), vec.end());
	}
	size_t getCountTables() const noexcept
	{
		return static_cast<const ConcreteDatabase*>(this)->getCountTablesImpl();
	}
private:
};
#endif

template<typename Storage>
struct SQLCookbookDb : public SchemaManager<SQLCookbookDb<Storage>, Storage>
{
	SQLCookbookDb(Storage& storage) : SchemaManager<SQLCookbookDb, Storage>(storage) {}

#if 0
	using ListOfTables = std::tuple<Artist, Department, Album, Employee, EmpBonus>;
	auto getDropOrderImpl() const noexcept
	{
		auto newTuple = TableOrder::tuple_divide<Employee, Department, ListOfTables>::convert();
		auto newerTuple = TableOrder::tuple_divide<EmpBonus, Employee, decltype(newTuple)>::convert();
		auto newestTuple = TableOrder::tuple_divide<Album, Artist, decltype(newerTuple)>::convert();
		return newestTuple;
	}
	size_t getCountTablesImpl() const noexcept
	{
		return std::tuple_size_v<ListOfTables>;
	}
#endif
};

