#pragma once


#include <sqlite_orm/sqlite_orm.h>
#include <memory>
#include <string>
#include <date/date.h>
#include <FixedPoint/Money.h>

#include "Model.DateBinding.h"
#include "Model.Colones.Binding.h"
#include "Model.DateBinding.h"
#include "Model.CoinBinding.h"
#include "Model.Dolares.Binding.h"
#include "Model.AccountType.Binding.h"


namespace Model
{
	template<typename T>
	using nullable_field = std::shared_ptr<T>;

	template<typename T>
	nullable_field<T> nullable(const T& t)
	{
		return std::make_shared<T>(t);
	}

	template<typename Data, typename...Pks>
	nullable_field<Data> fromNullablePKs(Pks...pks)
	{
		if ((pks && ...))
		{
			auto instance = Storage::getStorage().get<Data>(*pks...);
			return Model::nullable(instance);
		}
		return nullptr;
	}


#define PK
#define FK(Table)

	// Forwards:
	struct StatementLine;
	struct Responsible;
	struct LineResponsibility;
	struct Account;
	struct Category;
	struct Concept;
	struct Person;
	struct Statement;


	struct Category
	{
		PK std::string					m_name_id;
		bool						m_real_expense_or_income;
	};

	struct Person					// Leslie, Juan Jr, Juan Sr, Other
	{
		PK int m_id;
		std::string m_first_name;
		std::string m_last_name;
		std::string m_company_name;
	};


	/**
	 * \brief Can be one of my credit card or bank accounts, or can be Bank accounts of other people or companies
	 */
	struct Account
	{
		PK std::string m_number_id;
		FK(Person) int m_owner_fid;
		std::string	m_cuenta_cliente;
		Coin		m_currency;
		std::string m_description;
		AccountType	m_type;

		void AssignFK(const Person& person)
		{
			m_owner_fid = person.m_id;
		}
		Person getPerson();
	};

	struct Concept
	{
		PK std::string m_concept_id;
		FK(Category)  std::string m_category_name_fid;
		FK(Account)	  Model::nullable_field<std::string> m_account_payment_fid;		// TODO
		bool m_always;		// always apply this mapping rather than just suggesting
		bool m_is_regex;	// 

		void AssignFK(const Category& cat)
		{
			m_category_name_fid = cat.m_name_id;
		}
		void AssignFK(const Account& act)
		{
			auto id = Model::nullable(act.m_number_id);
			m_account_payment_fid = id;
		}
		Category getCategory();
		//{
		//	auto cat = ORM::Storage::getStorage().get<Category>(m_category_name_fid);
		//	return cat;
		//}

		// nullable navigation property:
		Model::nullable_field<Account> getAccount();
	};

	struct Responsible
	{
		PK int m_id;
		FK(Person) int m_person_fid;
		double m_percentage;	// degree of responsibility
		void Assign(const Person& person)
		{
			m_person_fid = person.m_id;
		}
		Person getPerson();
		//{
		//	auto person = ORM::Storage::getStorage().get<Person>(m_person_fid);
		//	return person;
		//}
	};

	// N:M
	struct LineResponsibility
	{
		FK(StatementLine) unsigned long m_statement_fid;
		FK(Responsible)   int m_responsible_fid;

		void Assign(const StatementLine& stmtLine);
		//{
		//	m_statement_fid = stmtLine.m_id;
		//}
		void Assign(const Responsible& resp);
		//{
		//	m_responsible_fid = resp.m_id;
		//}
		StatementLine getStatementLine();
		//{
		//	auto stmt = ORM::Storage::getStorage().get<StatementLine>(m_statement_fid);
		//	return stmt;
		//}
		Responsible getResponsible();
		//{
		//	auto stmt = ORM::Storage::getStorage().get<Responsible>(m_responsible_fid);
		//	return stmt;
		//}
	};

	struct Statement
	{
		std::string				m_fileName;
		std::string				m_filePath;
		date::sys_days			m_statementDate;
		FK(Account) std::string		m_account_fid;

		void Assign(const Account& act);
		//{
		//	m_account_fid = act.m_number_id;
		//}
		Account getAccount();
		//{
		//	auto act = ORM::Storage::getStorage().get<Account>(m_account_fid);
		//	return act;
		//}
	};



	struct StatementLine
	{
		PK unsigned long	m_id;
		FK(Account) std::string		m_account_fid;
		date::sys_days				m_lineDate;
		FK(Concept) std::string		m_concept_fid;
		Colones						m_amountInLocal;
		Dolares						m_amountInDollars;

		FK(Category)				std::shared_ptr<std::string>	m_category_fid;
		bool						m_enabled;
		std::string					m_details;
		FK(Account)					std::shared_ptr<std::string>  m_payment_to_fid;
		date::sys_days				m_statement_date;

		void AssignBelonging(const Account& act);
		//{
		//	m_account_fid = act.m_number_id;
		//}
		void Assign(const Concept& con);
		//{
		//	m_concept_fid = con.m_concept_id;
		//}
		void Assign(const Category& cat);
		//{
		//	auto catid = std::make_shared<std::string>(cat.m_name_id);
		//	m_category_fid = catid;
		//}
		void AssignPayment(const Account& act);
		//{
		//	auto actId = std::make_shared<std::string>(act.m_number_id);
		//	m_payment_to_fid = actId;
		//}
		Account getAccountBelonging();
		//{
		//	auto act = ORM::Storage::getStorage().get<Account>(m_account_fid);
		//	return act;
		//}
		Concept getConcept();
		//{
		//	auto conc = ORM::Storage::getStorage().get<Concept>(m_concept_fid);
		//	return conc;
		//}
		std::shared_ptr<Category> getCategory();
		//{
		//	if (m_category_fid)
		//	{
		//		auto cat = ORM::Storage::getStorage().get<Category>(*m_category_fid);
		//		auto shared_cat = std::make_shared<Category>(cat);
		//		return shared_cat;
		//	}
		//	return nullptr;
		//}
	};


}
