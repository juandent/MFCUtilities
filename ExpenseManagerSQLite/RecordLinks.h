#pragma once


//#include "Data_Tier.h"
struct Statement;
struct Concepto;
struct Account;
struct Pais;
struct Banco;
struct AccountOwner;
struct Transaccion;
struct Categoria;


class RecordLinks
{
	// template<int...Counts>
	// static bool allNonZero(Counts...counts);
public:
	static bool has_links(const Pais& pais);
	static bool has_links(const AccountOwner& owner);
	static bool has_links(const Concepto& concepto);
	static bool has_links(const Statement& statement);
	static bool has_links(const Categoria& categoria);
	static bool has_links(const Transaccion& trans);
	static bool has_links(const Account& acct);
	static bool has_links(const Banco& banco);
	static bool foreignKeysExist(const Statement& statement);
	static bool foreignKeysExist(const Pais& pais);
	static bool foreignKeysExist(const AccountOwner& owner);
	static bool foreignKeysExist(const Concepto& concepto);
	static bool foreignKeysExist(const Categoria& categoria);
	static bool foreignKeysExist(const Transaccion& trans);
	static bool foreignKeysExist(const Account& acct);
	static bool foreignKeysExist(const Banco& banco);

};


