#pragma once
#include "PersistentClasses.h"


struct Location;
struct Password;


class RecordLinks
{
	// template<typename T, T ...Counts>
	// static bool allNonZero(T...counts)
	// {
	// 	bool all_none_zero = (counts && ...);
	// }
	template<typename ...Counts>
	static bool allNonZero(Counts ... counts) requires (std::is_same_v<Counts, int> && ...)
	{
		//		static_assert((std::is_same_v<Counts, int> && ...));
		return (counts && ...);
	}
#if 1
	template<size_t N>
	static bool anyNonZero(int(&vec)[N])
	{
		return std::any_of(std::begin(vec), std::end(vec), [](int i) { return i != 0; });
	}
#endif
public:
	static bool has_links(const Location& location);
	static bool has_links(const Password& password);

	static bool foreignKeysExist(const Location& location);
	static bool foreignKeysExist(const Password& password);

};
