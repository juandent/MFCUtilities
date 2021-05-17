#pragma once


#undef min
#undef max

#include <optional>


#include <sqlite_orm/sqlite_orm.h>
// #include <catch2/catch.hpp>


#include "PersistentClasses.h"


inline 	auto& get_storage()
{
	using namespace sqlite_orm;
	using namespace std;

	static int flag = 0;

	constexpr const char* db_name{ "Local.sqlite" };


	static auto storage =
		make_storage(db_name,
			make_table("Claims",
				make_column("id_claim", &Claim::id, autoincrement(), primary_key()),
				make_column("fkey_patient", &Claim::fkey_patient),
				make_column("fkey_doctor", &Claim::fkey_doctor),
				make_column("fkey_medication", &Claim::fkey_medication),
				make_column("asprose_claim_number", &Claim::asprose_claim_number),
				make_column("asprose_case_number", &Claim::asprose_case_number),
				make_column("asprose_amount_presented", &Claim::asprose_amount_presented),
				make_column("ins_claim_number", &Claim::ins_claim_number),
				make_column("asprose_acknowledgement_type", &Claim::asprose_acknowledgement_type),
				make_column("comment", &Claim::comment),
				make_column("amount", &Claim::amount),
				make_column("other_system_id", &Claim::other_system_id),
				make_column("status", &Claim::status),
				foreign_key(&Claim::fkey_doctor).references(&Doctor::id),
				foreign_key(&Claim::fkey_medication).references(&Medication::id),
				foreign_key(&Claim::fkey_patient).references(&Patient::id)),
			make_table("Patients",
				make_column("id_patient", &Patient::id, autoincrement(), primary_key()),
				make_column("first_name", &Patient::first_name, collate_nocase()),
				make_column("last_name", &Patient::last_name, collate_nocase())),
			make_table("Doctors",
				make_column("id_doctor", &Doctor::id, autoincrement(), primary_key()),
				make_column("first_name", &Doctor::first_name, collate_nocase()),
				make_column("last_name", &Doctor::last_name),
				make_column("fkey_specialty", &Doctor::fkey_specialty),
				foreign_key(&Doctor::fkey_specialty).references(&Specialty::id)),
			make_table("Specialties",
				make_column("id_specialty", &Specialty::id, autoincrement(), primary_key()),
				make_column("name", &Specialty::name, collate_nocase())),
			make_table("Medications",
				make_column("id_medication", &Medication::id, autoincrement(), primary_key()),
				make_column("name", &Medication::name, collate_nocase())),
			make_table("Invoices",
				make_column("id_invoice", &Invoice::id, autoincrement(), primary_key()),
				make_column("fkey_claim", &Invoice::fkey_claim),
				make_column("number", &Invoice::number),
				make_column("amount", &Invoice::amount),
				make_column("type", &Invoice::type),
				make_column("description", &Invoice::description),
				make_column("fkey_INSResponse", &Invoice::fkey_INSResponse),
				foreign_key(&Invoice::fkey_claim).references(&Claim::id),
				foreign_key(&Invoice::fkey_INSResponse).references(&INSResponse::id)),
			make_table("INSResponses",
				make_column("id_INSResponse", &INSResponse::id, autoincrement(), primary_key()),
				make_column("liquidacion_num", &INSResponse::liquidacion_num),
				make_column("numero_caso", &INSResponse::numero_caso),
				make_column("total_bruto", &INSResponse::total_bruto),
				make_column("otras_deducciones", &INSResponse::otras_deducciones),
				make_column("copagos", &INSResponse::copagos),
				make_column("coaseguros", &INSResponse::coaseguros),
				make_column("deducible_anual", &INSResponse::deducible_anual),
				make_column("total_neto", &INSResponse::total_neto),
				make_column("retencion", &INSResponse::retencion),
				make_column("total_a_pagar", &INSResponse::total_a_pagar),
				make_column("comentario", &INSResponse::comentarios),
				make_column("tipo_cambio", &INSResponse::tipo_cambio)),
			make_table("INSResponseLine",
				make_column("id_INSResponseLine", &INSResponseLine::id, autoincrement(), primary_key()),
				make_column("fkey_INSResponse", &INSResponseLine::fkey_INSResponse),
				make_column("fkey_factura", &INSResponseLine::fkey_factura),
				make_column("monto_cubierto", &INSResponseLine::monto_cubierto),
				make_column("deducciones", &INSResponseLine::deducciones),
				make_column("copago", &INSResponseLine::copago),
				make_column("coaseguros", &INSResponseLine::coaseguros),
				make_column("deducible_anual", &INSResponseLine::deducible_anual),
				make_column("total_rubro_factura", &INSResponseLine::total_rubro_factura),
				make_column("porcentaje_de_monto_cubierto", &INSResponseLine::porcentaje_de_monto_cubierto),
				make_column("porcentaje_de_factura_cubierto", &INSResponseLine::porcentaje_de_factura_cubierto),
				foreign_key(&INSResponseLine::fkey_INSResponse).references(&INSResponse::id),
				foreign_key(&INSResponseLine::fkey_factura).references(&Invoice::id)));


	if (flag == 0)
	{
		flag = 1;
		storage.sync_schema(false);
	}

	return storage;
}

class Storage
{
public:
	static void fill_db_with_test_data();
};
