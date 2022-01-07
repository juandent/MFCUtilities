#include "pch.h"

import Util;



#include "RecordLinks.h"
#include "Data.h"




#if 1
#include "..\ORM_Extensions/Connections.h"
#else
#include <exception>

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

	static void setRecord( const Table* const record)
	{
		tableData = record;
	}
	static constexpr int getKeyValue()
	{
		if (tableData == nullptr) throw std::exception{ "tableData is null" };
		return tableData->*Key;
	}
private:
	inline static const Table* tableData = nullptr;
};


template <typename T, typename ...RefBy>
struct TableDef
{
	using Target = T;
	static std::tuple<RefBy...> reference_list;

	static bool has_links(const typename Target::Table& targetRec)
	{
		Target::setRecord(&targetRec);
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
		
		bool has = storage.count<DependentClass::Table>(where(is_equal(DependentClass::Key, Target::getKeyValue()))) > 0;

		return has || has_links<index - 1>();
	}
	template<>
	static bool has_links<0>()
	{
		return false;
	}

};

// connection between 2 TableKey's:
// Dependent pair
//		DepPair::Table = Rendimiento
//		DepPair::Key =   getKeyValue()
// Target pair
//		TargetPair::Table = Fondo
//		TargetPair::Key = &Fondo.id

template <typename DepPair, typename TargetPair>
struct TableConnection
{
	using Dependent = DepPair;
	using Target = TargetPair;

	static bool foreignKeyExists(const typename Dependent::Table& dep)
	{
		Dependent::setRecord(&dep);

		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		int count = storage.count<typename Target::Table>(where(is_equal(  Target::Key, Dependent::getKeyValue())));
		return count > 0;
	}
};


// T and Conns are TableConnection instances

template <typename T, typename ...Conns>
struct TableConnections
{
	using DependentTable = typename T::Dependent::Table;
	static std::tuple<T, Conns...> connections_list;

	static bool foreignKeysExist(const DependentTable& dep)
	{
		constexpr size_t size = std::tuple_size_v<decltype(connections_list)>;
		return foreignKeyExists<size>(dep);
	}
private:
	template<size_t index>
	static bool foreignKeyExists(const DependentTable& dep)
	{
		// is a TableConnection
		using Connection = std::tuple_element_t<index - 1, decltype(connections_list)>;
		bool exists = Connection::foreignKeyExists(dep);

		return exists && foreignKeyExists<index - 1>(dep);
	}
	template<>
	static bool foreignKeyExists<0>(const DependentTable& dep)
	{
		return true;
	}
};
#endif

namespace FondoLinks
{
	using FondoDef = TableKey<Fondo, &Fondo::id>;
	using Dep1 = TableKey<Inversion, &Inversion::fkey_fondo>;
	using Dep2 = TableKey<Rendimiento, &Rendimiento::fkey_fondo>;
	using FondoDependents = TableDef<FondoDef, Dep1, Dep2>;
}

#if 0
void use()
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();


	using FondoDef = TableKey<Fondo, &Fondo::id>;

	auto all = storage.get_all<Fondo>();

	FondoDef::Table rec = all[0];


	FondoDef::setRecord(&rec);
	int val = FondoDef::getKeyValue();
	// FondoDef::setKeyValue(val);


	using Dep1 = TableKey<Inversion, &Inversion::fkey_fondo>;
	using Dep2 = TableKey<Rendimiento, &Rendimiento::fkey_fondo>;

	using FondoDependents = TableDef<FondoDef, Dep1, Dep2>;
	// bool has = FondoDependents::has_links(rec);
}
#endif


bool RecordLinks::has_links(const Fondo& fondo)
{
	return FondoLinks::FondoDependents::has_links(fondo);
}

bool RecordLinks::has_links(const Rendimiento& rend)
{
	return false;
}

bool RecordLinks::has_links(const Inversion& inv)
{
	return false;
}

// Given a Dependent (Rendimiento or Inversion), all its foreign keys exist in target tables?
namespace RendimientoFKs
{
	using DepKey1 = TableKey<Rendimiento, &Rendimiento::fkey_fondo>;
	using TargetKey1 = TableKey<Fondo, &Fondo::id>;
	using Conn1 = TableConnection<DepKey1, TargetKey1>;

	using FKConnections = TableConnections<Conn1>;

#if 0
	void use()
	{
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto all = storage.get_all<Rendimiento>();
		const auto& rec = all[0];

		bool fkExists = Conn1::foreignKeyExists(rec);

		bool fkExistsInAll = FKConnections::foreignKeysExist(rec);

		std::ignore = fkExistsInAll;
	}
#endif
}

bool RecordLinks::foreignKeysExist(const Rendimiento& rend)
{
	return RendimientoFKs::FKConnections::foreignKeysExist(rend);
}

namespace InversionFKs
{
	using DepKey1 = TableKey<Inversion, &Inversion::fkey_fondo>;
	using TargetKey1 = RendimientoFKs::TargetKey1;
	using Conn1 = TableConnection<DepKey1, TargetKey1>;
	using Connections = TableConnections<Conn1>;
}



bool RecordLinks::foreignKeysExist(const Inversion& inv)
{
	return InversionFKs::Connections::foreignKeysExist(inv);
}

bool RecordLinks::foreignKeysExist(const Fondo& fondo)
{
	return true;	// no fkeys in this table
}

