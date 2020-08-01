#pragma once
#include "Data_Tier.h"

class ObjectBuilder
{
	std::map<std::string, std::unique_ptr<Account>>  m_accounts;
	
public:
	ObjectBuilder()
	{}

	std::unique_ptr<Account> getOwnAccount(std::string number);
	
};
