#pragma once
#include "PersistentClasses.h"


struct Fondo;
struct Inversion;
struct Rendimiento;


class RecordLinks
{
	template<typename ...Counts>
	static bool allNonZero(Counts ... counts) requires (std::is_same_v<Counts, int> && ...)
	{
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
	static bool has_links(const Fondo& fondo);
	static bool has_links(const Inversion& inversion);
	static bool has_links(const Rendimiento& rendimiento);

	static bool foreignKeysExist(const Fondo& fondo);
	static bool foreignKeysExist(const Inversion& inversion);
	static bool foreignKeysExist(const Rendimiento& rendimiento);

};
