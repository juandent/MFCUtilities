#include "pch.h"

import Util;
//#include "Util.h"


#include "RecordLinks.h"
#include "Data.h"


bool RecordLinks::has_links(const Location& location)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Password>(where(is_equal(&Password::fkey_location, location.id)));

	return count > 0;
}

bool RecordLinks::has_links(const Password& password)
{
	return false;
}



bool RecordLinks::foreignKeysExist(const Password& password)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count = storage.count<Location>(where(is_equal(&Location::id, password.fkey_location)));

	return count != 0;
}

bool RecordLinks::foreignKeysExist(const Location& location)
{
	using namespace sqlite_orm;

	return true;
}


