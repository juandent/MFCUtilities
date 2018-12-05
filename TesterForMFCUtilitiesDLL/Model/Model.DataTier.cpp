#include "stdafx.h"
#include "Model.DataTier.h"
#include "Model.ORM.Definitions.h"

using namespace Model;

Person Model::Account::getPerson()
{
	auto person = ORM::Storage::getStorage().get<Person>(m_owner_fid);
	return person;
}
