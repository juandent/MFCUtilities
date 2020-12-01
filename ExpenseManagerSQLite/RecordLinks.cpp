#include "stdafx.h"

import Util;


#include "RecordLinks.h"
#include "Data_Tier.h"


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

bool RecordLinks::has_links(const Banco& banco)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	auto count = storage.count<Account>(where(is_equal(&Account::fkey_bank, banco.id_bank)));
	
	return count > 0;
}

bool RecordLinks::foreignKeysExist(const Statement& statement)
{
	return true;
}


bool RecordLinks::foreignKeysExist(const Pais& pais)
{
	return true;
}

bool RecordLinks::foreignKeysExist(const AccountOwner& owner)
{
	return true;
}

bool RecordLinks::foreignKeysExist(const Concepto& concepto)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();

	int count = storage.count<Account>(where(is_equal(&Account::id_account, concepto.fkey_account)));
	return count != 0;
}

bool RecordLinks::foreignKeysExist(const Categoria& categoria)
{
	return true;
}

bool RecordLinks::foreignKeysExist(const Transaccion& trans)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();
	int count[5];	
	count[2] = 1;   // init to one
	
	count[0] = storage.count<Concepto>(where(is_equal(&Concepto::id_concepto, trans.fkey_concepto)));
	count[1] = storage.count<Account>(where(is_equal(&Account::id_account, trans.fkey_account_own)));
	if (trans.fkey_account_other)
	{
		count[2] = storage.count<Account>(where(is_equal(&Account::id_account, *trans.fkey_account_other)));
	}
	count[3] = storage.count<Categoria>(where(is_equal(&Categoria::id_categoria, trans.fkey_category)));
	count[4] = storage.count<Statement>(where(is_equal(&Statement::id_statement, trans.fkey_statement)));

	return allNonZero(count[0], count[1], count[2], count[3], count[4]);
//	return count[0] && count[1] && count[2] && count[3] && count[4];
}

bool RecordLinks::foreignKeysExist(const Account& acct)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();
	int count[2];

	count[0] = storage.count<AccountOwner>(where(is_equal(&AccountOwner::id_owner, acct.fkey_account_owner)));
	count[1] = storage.count<Banco>(where(is_equal(&Banco::id_bank, acct.fkey_bank)));
	
	return count[0] && count[1];
}

bool RecordLinks::foreignKeysExist(const Banco& banco)
{
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();
	int count;
	count = storage.count<Pais>(where(is_equal(&Pais::id_pais, banco.fkey_pais)));

	return count != 0;
}
