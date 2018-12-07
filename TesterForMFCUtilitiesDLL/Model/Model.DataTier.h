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

		// nullable navigation property:
		Model::nullable_field<Account> getAccount();
	};

	struct Responsible
	{
		PK int m_id;
		FK(Person) int m_person_fid;
		double m_percentage;	// degree of responsibility
		void AssignFK(const Person& person)
		{
			m_person_fid = person.m_id;
		}
		Person getPerson();
	};

	// N:M
	struct LineResponsibility
	{
		FK(StatementLine) unsigned long m_statement_fid;
		FK(Responsible)   int m_responsible_fid;

		void AssignFK(const StatementLine& stmtLine);
		void AssignFK(const Responsible& resp);
		StatementLine getStatementLine();
		Responsible getResponsible();
	};

	struct Statement
	{
		std::string				m_fileName;
		std::string				m_filePath;
		date::sys_days			m_statementDate;
		FK(Account) std::string		m_account_fid;

		void AssignFK(const Account& act);
		Account getAccount();
	};



	struct StatementLine
	{
		PK unsigned long	m_id;
		FK(Account) std::string		m_account_fid;
		date::sys_days				m_lineDate;
		FK(Concept) std::string		m_concept_fid;
		Colones						m_amountInLocal;
		Dolares						m_amountInDollars;

		FK(Category)				nullable_field<std::string>	m_category_fid;
		bool						m_enabled;
		std::string					m_details;
		FK(Account)					nullable_field<std::string>  m_payment_to_fid;
		date::sys_days				m_statement_date;

		void AssignBelongingFK(const Account& act);
		void AssignFK(const Concept& con);
		void AssignFK(const Category& cat);
		void AssignPaymentFK(const Account& act);
		Account getAccountBelonging();
		Concept getConcept();
		nullable_field<Category> getCategory();
	};


}
