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

bool RecordLinks::has_links(const Statement& statement)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Transaccion>(where(is_equal(&Transaccion::fkey_statement, statement.id_statement)));

	return count > 0;

}

bool RecordLinks::has_links(const Categoria& categoria)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Transaccion>(where(is_equal(&Transaccion::fkey_category, categoria.id_categoria)));

	return count > 0;
}

bool RecordLinks::has_links(const Transaccion& trans)
{
	return false;
}

bool RecordLinks::has_links(const Account& acct)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count[3]{};
	
	count[0] = storage.count<Transaccion>(where(is_equal(&Transaccion::fkey_account_own, acct.id_account)));
	count[1] = storage.count<Transaccion>(where(is_equal(&Transaccion::fkey_account_other, acct.id_account)));
	count[2] = storage.count<Concepto>(where(is_equal(&Concepto::fkey_account, acct.id_account)));
	
	return (count[0] + count[1] + count[2]) > 0;
}
