#pragma once

using namespace std::string_literals;

enum class AcknowledgementType
{
	None, Partial, Full
};


struct Claim
{
	int id;
	int fkey_patient;
	int fkey_doctor;
	int fkey_medication;					// for chronic meds
	date::sys_days start_date;
	date::sys_days submission_date;
	std::string asprose_claim_number;				// Claim: 85481
	std::string asprose_case_number;		// Case : 21INSENE0085
	double asprose_amount_presented;		// in colones
	std::string ins_claim_number;			// 202001017481-ACCSA0120094110
	int asprose_acknowledgement_type;		// full, partial, none: TODO: create an enum instead
	std::string comment;
	double amount;

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + Util::to_string(start_date) + " - "s + Util::to_string(submission_date) + " " + std::to_string(fkey_patient);
		return str;
	}

};


struct Patient
{
	int id;
	std::string first_name;
	std::string last_name;

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
	int number;
	double amount;
	int type;
	std::string description;
	std::optional<int> fkey_INSResponse;

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + std::to_string(number) + " "s + amountInColones();
		str += " "s + get_type();
		return str;
	}

	std::string amountInColones() const
	{
		Util::Colones col{amount};
		auto col_str = Util::to_string(col);
		return col_str;
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
	std::string liquidacion_num;
	long numero_caso;
	double total_bruto;
	double otras_deducciones;
	double copagos;
	double coaseguros;
	double deducible_anual;
	double total_neto;
	double retencion;
	double total_a_pagar;
	std::string comentarios;

	std::string simple_dump() const
	{
		std::string str = std::to_string(id) + " - "s + liquidacion_num + " "s + std::to_string(total_a_pagar);
		return str;
	}
};
/////////////////////////////////////////////////////////////////
///
///

