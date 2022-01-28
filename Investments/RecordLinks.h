#pragma once
#include "PersistentClasses.h"
#include "..\ORM_Extensions/Connections.h"


// struct Fondo;
// struct Inversion;
// struct Rendimiento;


class RecordLinks
{
	// TableKeys
	using FondoTable = TableKey<Fondo, &Fondo::id>;
	using InversionTable = TableKey<Inversion, &Inversion::fkey_fondo>;
	using RendimientoTable = TableKey<Rendimiento, &Rendimiento::fkey_fondo>;

	// Fondo structure
	struct Fondos
	{
		using PKDependents = TableDef<FondoTable, InversionTable, RendimientoTable>;
	};

	// Rendimiento structure
	struct Rendimientos
	{
		using Conn1 = TableConnection<RendimientoTable, FondoTable>;
		using FKConnections = TableConnections<Conn1>;
	};

	// Inversion structure
	struct Inversiones
	{
		using Conn1 = TableConnection<InversionTable, FondoTable>;
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
