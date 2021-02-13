#include "stdafx.h"

import Util;


#include "RecordLinks.h"
#include "Data.h"


bool RecordLinks::has_links(const Claim& claim)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Invoice>(where(is_equal(&Invoice::fkey_claim, claim.id)));


	return count > 0;
}

bool RecordLinks::has_links(const INSResponse& response)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Invoice>(where(is_equal(&Invoice::fkey_INSResponse, response.id)));

	return count > 0;
}


bool RecordLinks::has_links(const Doctor& doctor)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Claim>(where(is_equal(&Claim::fkey_doctor, doctor.id)));

	return count > 0;

}


bool RecordLinks::has_links(const Patient& patient)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Claim>(where(is_equal(&Claim::fkey_patient, patient.id)));

	return count > 0;

}

bool RecordLinks::has_links(const Medication& med)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Claim>(where(is_equal(&Claim::fkey_medication, med.id)));

	return count > 0;
}

bool RecordLinks::has_links(const Specialty& specialty)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Doctor>(where(is_equal(&Doctor::fkey_specialty, specialty.id)));

	return count > 0;
}

bool RecordLinks::has_links(const Invoice& )
{
	return false;
}

bool RecordLinks::foreignKeysExist(const Invoice& invoice)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count[2]{1,1};
	count[0] = storage.count<Claim>(where(is_equal(&Claim::id, invoice.fkey_claim)));
	if (invoice.fkey_INSResponse)
	{
		count[1] = storage.count<INSResponse>(where(is_equal(&INSResponse::id, invoice.fkey_INSResponse)));
	}
	
	return allNonZero(count[0], count[1]);
}


bool RecordLinks::foreignKeysExist(const Claim& claim)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count[3]{};
	count[0] = storage.count<Doctor>(where(is_equal(&Doctor::id, claim.fkey_doctor)));
	count[1] = storage.count<Patient>(where(is_equal(&Patient::id, claim.fkey_patient)));
	count[2] = storage.count<Medication>(where(is_equal(&Medication::id, claim.fkey_medication)));


	return allNonZero(count[0], count[1], count[2]);
}


bool RecordLinks::foreignKeysExist(const Doctor& doctor)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count = storage.count<Specialty>(where(is_equal(&Specialty::id, doctor.fkey_specialty)));

	return count > 0;
}

bool RecordLinks::foreignKeysExist(const Specialty& )
{
	return true;
}

bool RecordLinks::foreignKeysExist(const Patient& )
{
	return true;
}

bool RecordLinks::foreignKeysExist(const Medication& )
{
	return true;
}

bool RecordLinks::foreignKeysExist(const INSResponse& )
{
	return true;
}
