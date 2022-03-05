#include "stdafx.h"

import Util;


#include "Data_Tier.h"
#include "ObjectBuilder.h"



std::unique_ptr<Account> ObjectBuilder::getOwnAccount(std::string number)
{
	using namespace std;
	using namespace sqlite_orm;
	auto& storage = Storage::getStorage();
	
	auto it = m_accounts.find(number);
	if( it == m_accounts.end())
	{
		// get from storage
		auto clause = c(&Account::number) == number;

		auto vec = storage.select(columns(&Account::id_account, &Account::number), where(clause));
		if( vec.size() == 0 )
		{
			unique_ptr<Account> act { new Account{} };
			act->number = number;
			act->description;
			return act;
		}
	}
	return nullptr;
}

#if 0
struct Account
{
	int id_account;
	std::string number;				// 15103-02**-****-8467
	int fkey_bank;					// { BAC San Jose, "Barrio Dent", { Costa Rica} }
	int fkey_account_owner;			// { Juan Dent Herrera, ... }
	std::string description;		// AMEX Cashback Premium
	bool is_tarjeta;				// true
};
#endif

