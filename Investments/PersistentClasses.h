#pragma once

#include <sqlite_orm/sqlite_orm.h>


import Util;



using namespace std::string_literals;



struct Fondo
{
	int id;
	std::string abreviacion;
	std::string nombre;
	int tipo_cupon;

	enum TipoCupon
	{
		mensual = 1, trimestral = 3
	};


	std::string simple_dump() const;

	int num_participaciones_al(std::chrono::sys_days fecha) const noexcept;
	double get_rendimiento_unitario_al(std::chrono::year_month_day fecha) const noexcept;
};


struct Inversion
{
	int id;
	int num_participaciones;
	std::chrono::sys_days beginning_date;
	int fkey_fondo;

	std::string simple_dump() const;

	// relations
	Fondo getFondo() const;
};

struct Rendimiento
{
	int id;
	int fkey_fondo;
	double rendimiento_unitario;
	std::chrono::sys_days fecha;

	std::string simple_dump() const;

	// relations
	Fondo getFondo() const;
};


// type traits:
// list of persistent classes
template<typename T>
struct is_persistent
{
	static constexpr bool value = false;
};

template<>
struct is_persistent<Fondo>
{
	static constexpr bool value = true;
};

template<>
struct is_persistent<Rendimiento>
{
	static constexpr bool value = true;
};

template<>
struct is_persistent<Inversion>
{
	static constexpr bool value = true;
};



// struct X
// {
// 	int id;
// 	int fkey_Rendimiento;
// };

/////////////////////////////////////////////////////////////////
///
///

using namespace sqlite_orm;

using als_i = alias_i<Inversion>;
using als_r = alias_r<Rendimiento>;
using als_f = alias_f<Fondo>;

