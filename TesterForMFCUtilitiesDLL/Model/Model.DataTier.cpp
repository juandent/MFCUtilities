#include "stdafx.h"
#include "Model.DataTier.h"
#include "Model.ORM.Definitions.h"

using namespace Model;
using namespace ORM;

Person Model::Account::getPerson()
{
	auto person = ORM::Storage::getStorage().get<Person>(m_owner_fid);
	return person;
}



nullable_field<Account> Model::Concept::getAccount()
{
	return fromNullablePKs<Account>(m_account_payment_fid);
}
