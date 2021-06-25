#include "pch.h"

#include "PersistentClasses.h"

Claim::Claim()
{
	using namespace std::chrono;


	id = -1;
	amount = 0.0;
	asprose_acknowledgement_type = static_cast<int>(AcknowledgementType::None);
	asprose_amount_presented = 0.0;
	asprose_case_number = "";
	asprose_claim_number = "";
	comment = "";
	fkey_doctor = -1;
	fkey_medication = -1;
	fkey_patient = -1;
	amount = 0.0;
	ins_claim_number = "";
	const auto today = system_clock::now();

	start_date = floor<days>(today);
	submission_date = floor<days>(today);
	status = false;
	other_system_id = -1;

}
