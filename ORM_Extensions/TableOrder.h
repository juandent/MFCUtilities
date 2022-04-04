#pragma once

#include <tuple>

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
