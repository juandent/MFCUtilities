#pragma once
#include "PersistentClasses.h"
#include "..\ORM_Extensions/Connections.h"

struct Location;
struct Password;


class RecordLinks
{
	// TableKeys
	using LocationTable = TableKey<Location, &Location::id>;
	using PasswordTable = TableKey<Password, &Password::fkey_location>;

	struct Locations
	{
		using PKDependents = TableDef<LocationTable, PasswordTable>;
	};
	struct Passwords
	{
		// using Conn1 = TableConnection<PasswordTable, LocationTable>;
		using Conn1 = Locations::PKDependents::connection <PasswordTable> ::type;
		using FKConnections = TableConnections<Conn1>;
	};



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
	static bool has_links(const Location& location) { return Locations::PKDependents::has_links(location); }
	static bool has_links(const Password& password) { return false; }

	static bool foreignKeysExist(const Location& location) { return true; }
	static bool foreignKeysExist(const Password& password) { return Passwords::FKConnections::foreignKeysExist(password); }

};
