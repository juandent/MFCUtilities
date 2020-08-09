#pragma once


#include "Data_Tier.h"

class RecordLinks
{
public:
	static bool has_links(const Pais& pais);
	static bool has_links(const AccountOwner& owner);
	static bool has_links(const Concepto& concepto);
	static bool has_links(const Statement& statement);
	static bool has_links(const Categoria& categoria);
	static bool has_links(const Transaccion& trans);
	static bool has_links(const Account& acct);
	static bool has_links(const Banco& banco);
};


