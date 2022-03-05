#include "pch.h"
#include "PasswordLoader.h"

#include "Data.h"

void PasswordLoader::Load()
{
	m_csvFile.load(m_fileName);

	for (auto i = 0ull; i < m_csvFile.getRowCount(); ++i)
	{
		auto location = m_csvFile.getString({ i,0 });
		auto date = m_csvFile.getUSDate({ i,1 });
		auto password = m_csvFile.getString({ i,2 });


		auto location_id = getLocationId(location);

		createIfNotExist(password, date, location_id);

		i = i;
	}

}

int PasswordLoader::getLocationId(std::string locationName)
{
	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_l>(&Location::id)), where( c(alias_column<als_l>(&Location::name)) == locationName));

	if (otherlines.empty())
	{
		Location loc{ -1, locationName,""s, ""s };
		loc.id = Storage::getStorage().insert(loc);
		return loc.id;
	}

	else
	{
		int id = std::get<0>(otherlines[0]);
		return id;
	}
}

void PasswordLoader::createIfNotExist(std::string password, std::chrono::sys_days date, int location_id)
{

	auto otherlines = Storage::getStorage().select(columns(alias_column<als_p>(&Password::id)), where((c(alias_column<als_p>(&Password::fkey_location)) == location_id) and c(alias_column<als_p>(&Password::password)) == password));

	if (otherlines.empty())
	{
		Password pwd{ -1, password, date, location_id };
		pwd.id = Storage::getStorage().insert(pwd);
	}

}