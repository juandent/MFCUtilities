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
		// using FKConnections = typename FKDependents<FondoTable, Fondos::PKDependents>::construct::result;
	};

	// Rendimiento structure
	struct Rendimientos
	{
#if 1
		using FKConnections = typename FKDependents<RendimientoTable, Fondos::PKDependents>::construct::result;
#else
		using Conn1 = TableConnection<RendimientoTable, FondoTable>;
		using FKConnections = TableConnections<Conn1>;

		// pass TableKey we are interested in (RendimientoTable) to FKDependents
		// pass all existing TableDefs
		// search each TableDef looking for RendimientoTable not in first position  (search for RendimientoTable in TableDef::reference_list)
		// if found then
		//	create a  type TableConnection<RendimientoTable, TableDef::Target>
		//	append TableConnection to TableConnections
		//  FKDependents<RendimientoTable, Fondos::PKDependents, Fondos::PKDependents2>::TableConnections
		//	use bool TableDefinitions::foreignKeysExist(RendimientoTable param)
		static void use()
		{
			Rendimiento rendimiento;
			using coll = typename FKDependents<RendimientoTable, Fondos::PKDependents, Fondos::PKDependents2>::construct::result;

			bool ok = FKDependents<RendimientoTable, Fondos::PKDependents, Fondos::PKDependents2>::construct::result::foreignKeysExist(rendimiento);
			coll::foreignKeysExist(rendimiento);
			coll* p;
			// using list = TableConnectionList<coll>;
			// int count = pos;
		}
		using Conn1 = TableConnection<RendimientoTable, FondoTable>;
		using FKConnections = TableConnections<Conn1>;
#endif
	};

	// Inversion structure
	struct Inversiones
	{
		using FKConnections = typename FKDependents<InversionTable, Fondos::PKDependents>::construct::result;
	};
public:
	static bool has_links(const Fondo& fondo)
	{
		return Fondos::PKDependents::has_links(fondo);
	}
	static bool has_links(const Inversion& inversion) { return false; }
	static bool has_links(const Rendimiento& rendimiento) { return false; }

	static bool foreignKeysExist(const Fondo& fondo) { return true; }
	static bool foreignKeysExist(const Inversion& inversion) { return Inversiones::FKConnections::foreignKeysExist(inversion); }
	static bool foreignKeysExist(const Rendimiento& rendimiento) { return Rendimientos::FKConnections::foreignKeysExist(rendimiento); }
};
