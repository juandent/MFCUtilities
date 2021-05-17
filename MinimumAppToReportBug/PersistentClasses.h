#pragma once

// #include <sqlite_orm/sqlite_orm.h>
// #include <optional>


using namespace std::string_literals;

enum class AcknowledgementType
{
	None, Partial, Full
};

#define VERSION_2

struct Claim
{
	int id;
	int fkey_patient;
	int fkey_doctor;
	int fkey_medication;					// for chronic meds
	std::string asprose_claim_number;				// Claim: 85481
	std::string asprose_case_number;		// Case : 21INSENE0085
	double asprose_amount_presented;		// in colones
	std::string ins_claim_number;			// 202001017481-ACCSA0120094110
	int asprose_acknowledgement_type;		// full, partial, none: TODO: create an enum instead
	std::string comment;
	double amount;
	std::optional<int> other_system_id;
#ifdef VERSION_2
	int status;								// sent to asprose (true or false)
#endif

};


struct Patient
{
	int id;
	std::string first_name;
	std::string last_name;
};

struct Doctor
{
	int id;
	std::string first_name;
	std::string last_name;
	int fkey_specialty;
};

struct Specialty
{
	int id;
	std::string name;
};

struct Medication
{
	int id;
	std::string name;
};

enum class InvoiceType
{
	Appointment, Exam, Medication, Therapy, Procedure
};


struct Invoice
{
	int id;
	int fkey_claim;
	std::string number;
	double amount;
	int type;
	std::string description;
	std::optional<int> fkey_INSResponse;
};

struct INSResponse
{
	int id;
	std::string liquidacion_num;	// ACCSA0121008421
	long long numero_caso;				// 20200101
	double total_bruto;				// $291.47
	double otras_deducciones;		// 0.00
	double copagos;					// 0.00
	double coaseguros;				// $58.29
	double deducible_anual;			// 0.00
	double total_neto;				// CRC 142,134.87
	double retencion;				// 0.00
	double total_a_pagar;			// CRC 142,134.87
	std::string comentarios;		// deducible acumulado en su totalidad
	double tipo_cambio;				// 609.55
};

struct INSResponseLine
{
	int id;
	int fkey_INSResponse;
	int fkey_factura;
	//int fkey_claim;							// fkey_factura->fkey_claim
	double monto_cubierto;
	double deducciones;
	double copago;
	double coaseguros;
	double deducible_anual;
	double total_rubro_factura;
	double porcentaje_de_monto_cubierto;	// total_rubro_factura/monto_cubierto
	double porcentaje_de_factura_cubierto;	// total_rubro_factura/fkey_factura->amount
};

/////////////////////////////////////////////////////////////////
///
///

using namespace sqlite_orm;

using als_c = alias_c<Claim>;
using als_p = alias_p<Patient>;
using als_d = alias_d<Doctor>;
using als_s = alias_s<Specialty>;
using als_m = alias_m<Medication>;
using als_i = alias_i<Invoice>;
using als_j = alias_j<INSResponse>;
using als_k = alias_k<INSResponseLine>;
using als_l = alias_l<Claim>;
using als_q = alias_q<Invoice>;

