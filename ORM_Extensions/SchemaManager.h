#pragma once
#include <tuple>

#include "..\ORM_Extensions/update_schema.h"

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

template<typename ConcreteDatabase, typename Storage>
struct SchemaManager
{
	Storage& storage;
	update_schema<Storage> fk_controller;

	SchemaManager(Storage& storage) : storage { storage}
	{
	}
private:
#if 0
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
		bool exists = storage.table_exists(storage.tablename<Element>());
		if (exists)
		{
			bool fk = storage.pragma.foreign_keys();
			vec = storage.get_all<Element>();
			fk = storage.pragma.foreign_keys();
			storage.drop_table(storage.tablename<Element>());
			fk = storage.pragma.foreign_keys();
		}
		return vec;
	}
#endif
private:
	// this section will become private
	template<typename Element>
	auto load_drop()
	{
		assert(storage.foreign_key()== false);
		std::vector<Element> vec;
		bool exists = storage.table_exists(storage.tablename<Element>());
		if (exists)
		{
			vec = storage.get_all<Element>();
			storage.drop_table(storage.tablename<Element>());
		}
		return vec;
	}
	template<typename Vector>
	void replace(const Vector& vec)
	{
		assert(storage.foreign_key() == false);
		storage.replace_range(vec.begin(), vec.end());
	}
private:
#if 0
	size_t getCountTables() const noexcept
	{
		return static_cast<const ConcreteDatabase*>(this)->getCountTablesImpl();
	}
#endif
private:
	template<typename Element>
	void load_drop_sync_replace()
	{
		update_schema us{ storage };
		auto vec = load_drop<Element>();
		storage.sync_schema(true);
		replace( vec);
	}
	void guarded_sync_schema()
	{
		update_schema us{ storage };
		storage.sync_schema(true);
	}

public:
	// Column
	void remove_column()
	{
		guarded_sync_schema();
	}
	void add_column()		// must be nullable or with default value - attempt to load throws exception
	{
		guarded_sync_schema();
	}
	// Foreign Keys
	template<typename Element>
	void remove_fk_constraint()		// change not recognized
	{
		load_drop_sync_replace<Element>();
	}
	template<typename Element>
	void add_fk_constraint()		// change not recognized
	{
		load_drop_sync_replace<Element>();
	}
	// Primary Keys
	void remove_pk_constraint()		// change not recognized
	{
		guarded_sync_schema();
	}
	template<typename Element>
	void add_pk_constraint()		// change not recognized
	{
		load_drop_sync_replace<Element>();
	}
	// Nulls
	template<typename Element>
	void remove_null_constraint()	// change recognized but sync_schema() loses all data
	{
		load_drop_sync_replace<Element>();
	}
	template<typename Element>
	void add_null_constraint()		// change recognized but sync_schema() loses all data
	{
		load_drop_sync_replace<Element>();
	}
	// Default values				
	template<typename Element>
	void remove_default_value()		// change recognized but sync_schema() loses all data
	{
		load_drop_sync_replace<Element>();
	}
	template<typename Element>
	void add_default_value()		// change recognized but sync_schema() loses all data
	{
		load_drop_sync_replace<Element>();
	}
	// generated_always_as
	template<typename Element>
	void remove_generated_always_as()		// change in sync_schema() conflict on column duplicate name
	{
		load_drop_sync_replace<Element>();
	}
	template<typename Element>
	void add_generated_always_as()			// change not recognized, sync_schema() does not loose
	{
		load_drop_sync_replace<Element>();
	}
	// Unique constraint
	template<typename Element>
	void remove_unique_constraint()		// change not recognized
	{
		load_drop_sync_replace<Element>();
	}
	template<typename Element>
	void add_unique_constraint()		// change not recognized
	{
		load_drop_sync_replace<Element>();
	}


	// Verify uniqueness general case
	template<typename Element, typename sort_lambda, typename equality_lambda>
	auto find_duplicate(sort_lambda& sorter, equality_lambda& equality_pred )
	{
		auto vec = storage.get_all<Element>();
		std::sort(vec.begin(), vec.end(), sorter);
		auto it = std::adjacent_find(vec.begin(), vec.end(), equality_pred);
		return std::make_pair(it != vec.end(), it);
	}

	// verify uniqueness column case
	template<typename type, auto type::* key_col>
	auto find_duplicate()
	{
		auto vec = storage.get_all<type>();
		std::sort(vec.begin(), vec.end(), sort_order<type, key_col>());
		auto it = std::adjacent_find(vec.begin(), vec.end(), sort_equal<type, key_col>());
		return std::make_pair(it != vec.end(), it);
	}

	// Check constraints
	template<typename Element>
	void remove_check_constraint()		// change not recognized
	{
		load_drop_sync_replace<Element>();
	}
	template<typename Element>
	void add_check_constraint()		// change not recognized
	{
		load_drop_sync_replace<Element>();
	}

private:
	template<typename type, auto type::* key_col>
	struct sort_order
	{
		bool operator()(const type& lhs, const type& rhs)
		{
			return lhs.*key_col < rhs.*key_col;
		}
	};

	template<typename type, auto type::* key_col>
	struct sort_equal
	{
		bool operator()(const type& lhs, const type& rhs)
		{
			return lhs.*key_col == rhs.*key_col;
		}
	};

	// template<typename type, auto type::* key_col>
	// bool sort_order(const type& left, const type& right)
	// {
	// 	return left.*key_col < right.*key_col;
	// }
	// template<typename type, typename field_type, field_type type::* key_col>
	// bool sort_equal(const type& left, const type& right)
	// {
	// 	return left.*key_col == right.*key_col;
	// }

};



/*
 *
 *
 *
 * struct SQLCookbookDb : public SchemaManager<SQLCookbookDb>
{
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
};


SQLCookbookDb db;
		auto count = db.getCountTables();
		auto vec0 = db.drop_table<0>(storage);
		auto vec1 = db.drop_table<1>(storage);
		auto vec2 = db.drop_table<2>(storage);
		auto vec3 = db.drop_table<3>(storage);
		auto vec4 = db.drop_table<4>(storage);

		storage.sync_schema();

		db.replace_table(storage, vec4);
		db.replace_table(storage, vec3);
		db.replace_table(storage, vec2);
		db.replace_table(storage, vec1);
		db.replace_table(storage, vec0);

 */
