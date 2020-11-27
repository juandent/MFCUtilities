#pragma once


#include <sqlite_orm/sqlite_orm.h>
#include <memory>
#include <string>
#include <date/date.h>
#include <FixedPoint/Money.h>

// #include "Data_Tier.h"

#include "Model.DateBinding.h"
#include "Model.Colones.Binding.h"
#include "Model.DateBinding.h"
#include "Model.CoinBinding.h"
#include "Model.Dolares.Binding.h"
#include "Model.AccountType.Binding.h"


namespace Model
{
	struct Nullable
	{
		template<typename T>
		using Type = std::shared_ptr<T>;

		template<typename T>
		inline
			static Type<T> make_nullable(const T& t)
		{
			return std::make_shared<T>(t);
		}

		template<typename Data, typename...Fks>
		inline
			static
			Type<Data> getFromFKs(Fks...fks)
		{
			if ((fks && ...))
			{
				auto instance = Storage::getStorage().get<Data>(*fks...);
				return make_nullable(instance);
			}
			return nullptr;
		}
	};

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
	struct AccountStatement;


	struct Category
	{
		PK std::string					m_name_id;
		bool						m_real_expense_or_income;

		std::vector<StatementLine> getStatementLines();
		std::vector<StatementLine> getStatementLines(date::sys_days statement_date);
	};

	struct Person					// Leslie, Juan Jr, Juan Sr, Other
	{
		PK int m_id;
		std::string m_first_name;
		std::string m_last_name;
		std::string m_company_name;
		std::string get_full_name() { return m_first_name + " " + m_last_name; }
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

		void AssignPK(const std::string& id)
		{
			m_number_id = id;
			// determine if it is a credit card or a bank account
			auto pos = id.find_first_of("*");
			bool isCreditCard = pos != std::string::npos;
			m_type = isCreditCard ? AccountType::CreditCard : AccountType::BankAccount;

		}
		void SetCurrencyType(Dolares dollar_amount)
		{
			if (m_type == AccountType::BankAccount)
			{
				m_currency = dollar_amount > 0 ? Coin::Dolar : Coin::Colon;
			}
			else
			{
				m_currency = Coin::Both;
			}
		}
		void AssignFK(const Person& person)
		{
			m_owner_fid = person.m_id;
		}
		Person getOwner();
	};
	
	struct Concept
	{
		PK std::string m_concept_id;
		//FK(Category)  Nullable::Type<std::string> m_category_name_fid;
		FK(Account)	  Nullable::Type<std::string> m_account_payment_fid;		// TODO
		bool m_always;		// always apply this mapping rather than just suggesting
		bool m_is_regex;	// 

		//void AssignFK(const Category& cat)
		//{
		//	auto id = Nullable::make_nullable(cat.m_name_id);
		//	m_category_name_fid = id;
		//}
		void AssignFK(const Account& act)
		{
			auto id = Nullable::make_nullable(act.m_number_id);
			m_account_payment_fid = id;
		}
		//Nullable::Type<Category> getCategory();

		// nullable navigation property:
		Nullable::Type<Account> getAccount();
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
		FK(StatementLine) unsigned long m_statement_line_fid;
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
		PK date::sys_days			m_statementDate;
	};

	// TODO: not yet persisted!!
	struct AccountStatement
	{
		std::string				m_fileName;
		std::string				m_filePath;
		PK date::sys_days			m_statementDate;
		PK FK(Account) std::string	m_account_fid;

		void AssignFK(const Account& act);
		Account getAccount();
	};


	struct StatementLine
	{
		PK unsigned long	m_id;
		FK(Account) std::string		m_belongs_to_account_fid;
		date::sys_days				m_lineDate;
		FK(Concept) std::string		m_concept_fid;
		Colones						m_amountInLocal;
		Dolares						m_amountInDollars;

		FK(Category)				Nullable::Type<std::string>	m_category_fid;
		bool						m_enabled;
		std::string					m_details;
		FK(Account)					Nullable::Type<std::string>  m_refers_to_account_fid;
		date::sys_days				m_statement_date;

		void AssignBelongingFK(const Account& act);
		void AssignFK(const Concept& con);
		void AssignFK(const Category& cat);
		void AssignReferingFK(const Account& act);
		Account getAccountBelonging();
		Nullable::Type<Account> getAccountRefering();
		Concept getConcept();
		Nullable::Type<Category>	getCategory();
		Statement					getStatement();
		std::vector<Responsible>	getResponsibles();
		void						AddResponsible(const Person& person, double percentage = 1);
	};
}
