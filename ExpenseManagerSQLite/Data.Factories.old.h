// #include "stdafx.h"

#include "Data_Tier.h"

namespace Factory
{
	class ConceptoFactory
	{
		Concepto concepto;
	public:
		ConceptoFactory(const std::string& name)
			: concepto{ -1, name }
		{}
		Concepto insert()
		{
			concepto.id_concepto = Storage::getStorage().insert(concepto);
			return concepto;
		}
	};


	class AccountOwnerFactory
	{
		AccountOwner account_owner;
	public:
		AccountOwnerFactory(const std::string& name)
			: account_owner{ -1, name }
		{}
		AccountOwner insert()
		{
			account_owner.id_owner = Storage::getStorage().insert(account_owner);
			return account_owner;
		}
	};

	class BancoFactory
	{
		Banco banco;
	public:
		BancoFactory(const std::string& name, const std::string& ubicacion, const Pais& pais)
			: banco{ -1, name, ubicacion, pais.id_pais }
		{}
		Banco insert()
		{
			banco.id_bank = Storage::getStorage().insert(banco);
			return banco;
		}
	};

	class AccountFactory
	{
		Account account;
	public:
		AccountFactory(const std::string& number, const std::string& description, const AccountOwner& owner, const Banco& banco)
			: account{ -1, number, banco.id_bank, owner.id_owner, description }
		{
		}
		Account insert()
		{
			account.id_account = Storage::getStorage().insert(account);
			return account;
		}
	};


}

