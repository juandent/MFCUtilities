#pragma once

// import Util;
// #include "..\JDMFCLibrary/Util.h"


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
	std::chrono::sys_days start_date;
	std::chrono::sys_days submission_date;
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

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s; // +Util::to_string(start_date) + " - "s + Util::to_string(submission_date) + " " + std::to_string(fkey_patient);
		return str;
	}

	std::string dump() const;

	double get_total_amount();
};


struct Patient
{
	int id;
	std::string first_name;
	std::string last_name;

	std::string name() const
	{
		return first_name + " "s + last_name;
	}
	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + first_name + " "s + last_name;
		return str;
	}
};

struct Doctor
{
	int id;
	std::string first_name;
	std::string last_name;
	int fkey_specialty;

	std::string name() const
	{
		return first_name + " "s + last_name;
	}
	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + first_name + " "s + last_name;
		return str;
	}
};

struct Specialty
{
	int id;
	std::string name;

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + name;
		return str;
	}

};

struct Medication
{
	int id;
	std::string name;

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + name;
		return str;
	}
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

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + number + " "s + amountInColones();
		str += " "s + get_type();
		return str;
	}

	std::string amountInColones() const
	{
		// Util::Colones col{amount};
		// auto col_str = Util::to_string(col);
		// return col_str;
		return "";
	}
	
	std::string get_type() const
	{
		switch(static_cast<InvoiceType>(type))
		{
		case InvoiceType::Medication:
			return "Medicinas"s;
		case InvoiceType::Appointment:
			return "Consulta"s;
		case InvoiceType::Exam:
			return "Examenes"s;
		case InvoiceType::Procedure:
			return "Procedimiento"s;
		case InvoiceType::Therapy:
			return "Terapia";
		default:
			return "?"s;
		}
	}
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
	std::chrono::sys_days date_response;

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + liquidacion_num + " "s + amountInColones();
		return str;
	}
	std::string amountInColones() const
	{
		// Util::Colones col{ total_a_pagar};
		// auto col_str = Util::to_string(col);
		// return col_str;
		return "";
	}
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

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " : "s + std::to_string(fkey_INSResponse) + " - "s + std::to_string(fkey_factura);
		return str;
	}
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