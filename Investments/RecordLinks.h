#pragma once
#include "PersistentClasses.h"
#include "..\ORM_Extensions/Connections.h"


// struct Fondo;
// struct Inversion;
// struct Rendimiento;


class RecordLinks
{
	// Fondo structure
	struct Fondos
	{
		using FondoDef = TableKey<Fondo, &Fondo::id>;
		using Dep1 = TableKey<Inversion, &Inversion::fkey_fondo>;
		using Dep2 = TableKey<Rendimiento, &Rendimiento::fkey_fondo>;
		using PKDependents = TableDef<FondoDef, Dep1, Dep2>;
	};

	// Rendimiento structure
	struct Rendimientos
	{
		using DepKey1 = TableKey<Rendimiento, &Rendimiento::fkey_fondo>;
		using TargetKey1 = TableKey<Fondo, &Fondo::id>;
		using Conn1 = TableConnection<DepKey1, TargetKey1>;
		using FKConnections = TableConnections<Conn1>;
	};

	// Inversion structure
	struct Inversiones
	{
		using DepKey1 = TableKey<Inversion, &Inversion::fkey_fondo>;
		using TargetKey1 = Rendimientos::TargetKey1;
		using Conn1 = TableConnection<DepKey1, TargetKey1>;
		using FKConnections = TableConnections<Conn1>;
	};
public:
	static bool has_links(const Fondo& fondo)  { return Fondos::PKDependents::has_links(fondo); }
	static bool has_links(const Inversion& inversion) { return false; }
	static bool has_links(const Rendimiento& rendimiento) { return false; }

	static bool foreignKeysExist(const Fondo& fondo) { return true; }
	static bool foreignKeysExist(const Inversion& inversion) { return Inversiones::FKConnections::foreignKeysExist(inversion); }
	static bool foreignKeysExist(const Rendimiento& rendimiento) { return Rendimientos::FKConnections::foreignKeysExist(rendimiento); }
};
