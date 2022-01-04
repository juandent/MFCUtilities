#include "pch.h"

import Util;



#include "RecordLinks.h"
#include "Data.h"

#include "Typelist.h"


// Type: Fondo identified by: Fondo.id
// is referenced by	: Rendimiento using Rendimiento::fkey_fondo
//					: Inversion using Inversion::fkey_fondo
// 

template <typename T, int T::*K>
struct TableKey
{
	using Table = T;
	using KeyType = decltype(K);

	static constexpr KeyType Key = K;
	inline static Table* tableData = nullptr;
	// using Key = int T::*;
	// const Key f;

	// constexpr TableKey(Key val) : f{val} {}

	static constexpr int getKeyValue()
	{
		return tableData->*Key;
	}
	static constexpr void setKeyValue(int val)
	{
		tableData->*Key = val;
	}
};


template <typename Target, typename ...RefBy>
struct TableDef
{
	static Target target;
	static std::tuple<RefBy...> reference_list;


	// TableDef(Target target, RefBy... refs) : reference_list{ refs... }, target{target} {}

	static bool has_links()
	{
		constexpr size_t size = std::tuple_size_v<decltype(reference_list)>;

		bool has = has_links<size>();
		return has;
	}
private:
	template<size_t index>
	static bool has_links()
	{
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		// is a TableKey
		using DependentClass = std::tuple_element_t<index-1, decltype(reference_list)>;
		// DependentClass
		
		bool has = storage.count<DependentClass::Table>(where(is_equal(DependentClass::Key, Target::getKeyValue()))) > 0;

		return has || has_links<index - 1>();
	}
	template<>
	static bool has_links<0>()
	{
		return false;
	}

};




void use()
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();


	using FondoDef = TableKey<Fondo, &Fondo::id>;

	auto all = storage.get_all<Fondo>();

	FondoDef::Table rec = all[0];


	FondoDef::tableData = &rec;
	int val = FondoDef::getKeyValue();
	FondoDef::setKeyValue(val);


	using Dep1 = TableKey<Inversion, &Inversion::fkey_fondo>;
	using Dep2 = TableKey<Rendimiento, &Rendimiento::fkey_fondo>;

	using FondoDependents = TableDef<FondoDef, Dep1, Dep2>;
	bool has = FondoDependents::has_links();
}



bool RecordLinks::has_links(const Fondo& fondo)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count[2];
	count[0] = storage.count<Rendimiento>(where(is_equal(&Rendimiento::fkey_fondo, fondo.id)));
	count[1] = storage.count<Inversion>(where(is_equal(&Inversion::fkey_fondo, fondo.id)));

	return RecordLinks::anyNonZero(count);
}
#if 0
bool RecordLinks::has_links(const Password& password)
{
	return false;
}



bool RecordLinks::foreignKeysExist(const Password& password)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count = storage.count<Location>(where(is_equal(&Location::id, password.fkey_location)));

	return count != 0;
}

bool RecordLinks::foreignKeysExist(const Location& location)
{
	using namespace sqlite_orm;

	return true;
}


#endif
