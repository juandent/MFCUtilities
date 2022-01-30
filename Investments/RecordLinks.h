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
		using PKDependents2 = TableDef<FondoTable, InversionTable, RendimientoTable>;
	};

	// Rendimiento structure
	struct Rendimientos
	{
#if 0
		using Conn1 = Fondos::PKDependents::connection<RendimientoTable>::type;
		using FKConnections = TableConnections<Conn1>;
#endif
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
			int pos = FKDependents<RendimientoTable, Fondos::PKDependents, Fondos::PKDependents2>::findInTblDefs_list();
			int coll_size = FKDependents<RendimientoTable, Fondos::PKDependents, Fondos::PKDependents2>::collection_size();
			int count = pos;
		}
		using Conn1 = TableConnection<RendimientoTable, FondoTable>;
		using FKConnections = TableConnections<Conn1>;
	};

	// Inversion structure
	struct Inversiones
	{
		using Conn1 = Fondos::PKDependents::connection<InversionTable>::type;
		using FKConnections = TableConnections<Conn1>;
	};
public:
	static bool has_links(const Fondo& fondo)
	{
		Rendimientos::use();
		return Fondos::PKDependents::has_links(fondo);
	}
	static bool has_links(const Inversion& inversion) { return false; }
	static bool has_links(const Rendimiento& rendimiento) { return false; }

	static bool foreignKeysExist(const Fondo& fondo) { return true; }
	static bool foreignKeysExist(const Inversion& inversion) { return Inversiones::FKConnections::foreignKeysExist(inversion); }
	static bool foreignKeysExist(const Rendimiento& rendimiento) { return Rendimientos::FKConnections::foreignKeysExist(rendimiento); }
};
