#include "pch.h"

import Util;



#include "RecordLinks.h"
#include "Data.h"



#include "..\ORM_Extensions/Connections.h"

namespace FondoLinks
{
	using FondoDef = TableKey<Fondo, &Fondo::id>;
	using Dep1 = TableKey<Inversion, &Inversion::fkey_fondo>;
	using Dep2 = TableKey<Rendimiento, &Rendimiento::fkey_fondo>;
	using FondoDependents = TableDef<FondoDef, Dep1, Dep2>;
}

// prior to remove()
bool RecordLinks::has_links(const Fondo& fondo)
{
#undef JDH
#ifdef JDH
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	bool has_dependent_rows = storage.has_dependent_rows(fondo);
	std::ignore = has_dependent_rows;
#endif
	// return FondoLinks::FondoDependents::has_links(fondo);

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
}

bool RecordLinks::foreignKeysExist(const Rendimiento& rend)
{
#undef JDH
#ifdef JDH
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	bool has_dependent = storage.has_dependent_rows(rend);
	std::ignore = has_dependent;
#endif

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

