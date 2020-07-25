#include "stdafx.h"
#include "RecordLinks.h"



bool RecordLinks::has_links(const Pais& pais)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Banco>(where(is_equal(&Banco::fkey_pais, pais.id_pais)));
	
	return count > 0;
}




bool RecordLinks::has_links(const AccountOwner& owner)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Account>(where(is_equal(&Account::fkey_account_owner, owner.id_owner)));

	return count > 0;
}


bool RecordLinks::has_links(const Concepto& concepto)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Account>(where(is_equal(&Transaccion::fkey_concepto, concepto.id_concepto)));

	return count > 0;

}