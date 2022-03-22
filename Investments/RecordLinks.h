#pragma once
#include "PersistentClasses.h"
#include "..\ORM_Extensions/Connections.h"


// struct Fondo;
// struct Inversion;
// struct Rendimiento;


class RecordLinks
{
	// TableKeys
	using FondoPK = TableKey<Fondo, &Fondo::id>;
	using XPK = TableKey<X, &X::id>;
	using InversionFondoFK = TableKey<Inversion, &Inversion::fkey_fondo>;
	using RendimientoFondoFK = TableKey<Rendimiento, &Rendimiento::fkey_fondo>;
	using RendimientoXFK = TableKey<Rendimiento, &Rendimiento::fkey_x>;

	// Fondo structure
	struct Fondos
	{
		using PKDependents = PKDependencies<FondoPK, InversionFondoFK, RendimientoFondoFK>;
	};

	struct Xs
	{
		using PKDependents = PKDependencies<XPK, RendimientoXFK>;
	};

	// Rendimiento structure
	struct Rendimientos
	{
		using FKDependents = typename FKDependencies<RendimientoFondoFK, Fondos::PKDependents>::construct::result;
		using FKDependents1 = typename FKDependencies <RendimientoXFK, Xs::PKDependents>::construct::result;
	};

	// Inversion structure
	struct Inversiones
	{
		using FKDependents = typename FKDependencies<InversionFondoFK, Fondos::PKDependents>::construct::result;
	};
public:
	static bool has_links(const Fondo& fondo)	{	return Fondos::PKDependents::has_links(fondo);	}
	static bool has_links(const Inversion& inversion) { return false; }
	static bool has_links(const Rendimiento& rendimiento) { return false; }

	static bool foreignKeysExist(const Fondo& fondo) { return true; }
	static bool foreignKeysExist(const Inversion& inversion) { return Inversiones::FKDependents::foreignKeysExist(inversion); }
	static bool foreignKeysExist(const Rendimiento& rendimiento) {
		return Rendimientos::FKDependents::foreignKeysExist(rendimiento) &&
			Rendimientos::FKDependents1::foreignKeysExist(rendimiento);
	}
};
