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

std::shared_ptr<Account> Model::Concept::getAccount()
{
	if (m_account_payment_fid)
	{
		auto act = Storage::getStorage().get<Account>(*m_account_payment_fid);
		auto account = Storage::wrap(act);
		return account;
	}
	return nullptr;
}
