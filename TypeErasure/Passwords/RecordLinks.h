#pragma once
#include "PersistentClasses.h"
#include "..\ORM_Extensions/Connections.h"

struct Location;
struct Password;


class RecordLinks
{
	// TableKeys
	using LocationPK = TableKey<Location, &Location::id>;
	using PasswordLocationFK = TableKey<Password, &Password::fkey_location>;

	struct Locations
	{
		using PKDependents = PKDependencies<LocationPK, PasswordLocationFK>;
	};
	struct Passwords
	{
		using FKDependents = typename FKDependencies<PasswordLocationFK, Locations::PKDependents>::construct::result;
	};



	template<typename ...Counts>
	static bool allNonZero(Counts ... counts) requires (std::is_same_v<Counts, int> && ...)
	{
		//		static_assert((std::is_same_v<Counts, int> && ...));
		return (counts && ...);
	}

	template<size_t N>
	static bool anyNonZero(int(&vec)[N])
	{
		return std::any_of(std::begin(vec), std::end(vec), [](int i) { return i != 0; });
	}
public:
	static bool has_links(const Location& location) { return Locations::PKDependents::has_links(location); }
	static bool has_links(const Password& password) { return false; }

	static bool foreignKeysExist(const Location& location) { return true; }
	static bool foreignKeysExist(const Password& password) { return Passwords::FKDependents::foreignKeysExist(password); }

};
