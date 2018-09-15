#pragma once


#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <memory>
#include <string>
#include <date/date.h>
#include <FixedPoint/Money.h>

#include "CoinBinding.h"
#include "DateBinding.h"
#include "Colones.Binding.h"
#include "Dolares.Binding.h"

namespace DataTier
{
#define PK
#define FK(Table)

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
		FK(Person_Rec) int m_owner_fid;
		Coin		m_currency;
	};

	struct Concept
	{
		PK std::string m_concept_id;
		FK(Category_Rec)  std::string m_category_name_fid;
		FK(Account_Rec)	std::shared_ptr<std::string> m_account_payment_fid;		// TODO
		bool m_always;		// always apply this mapping rather than just suggesting
		bool m_is_regex;	// 
	};

	struct Responsible
	{
		PK int m_id;
		FK(Person_Rec) int m_person_fid;
		double m_percentage;	// degree of responsibility
	};

	// N:M
	struct LineResponsibility
	{
		FK(StatementLine_Rec) unsigned long m_statement_fid;
		FK(Responsible_Rec)   int m_responsible_fid;
	};

	struct Statement
	{
		std::string				m_fileName;
		std::string				m_filePath;
		date::sys_days			m_statementDate;
		FK(Account_Rec) std::string		m_account_fid;

#if 0
  		Statement(std::string filename, std::string filepath, date::sys_days statement_date, std::string account)
			: m_fileName{filename}, m_filePath{filepath}, m_statementDate{statement_date}, m_account_fid{account}
		{}
#endif

	};

	struct Statement_derived : Statement
	{
		int m_number;

#if 0
  Statement_derived(std::string filename, std::string filepath, date::sys_days statement_date, std::string account, int number )
			: Statement{filename , filepath, statement_date, account }, m_number{number}
		{}
#endif

	};

	struct StatementLine
	{
		PK unsigned long	m_id;
		FK(Account_Rec) std::string		m_account_fid;
		date::sys_days				m_lineDate;
		FK(Concept_Rec) std::string		m_concept_fid;
		Colones						m_amountInLocal;
		Dolares						m_amountInDollars;

		FK(Category_Rec) std::shared_ptr<std::string>	m_category_fid;
		bool						m_enabled;
		std::string					m_details;
		FK(Account_Rec) std::shared_ptr<std::string>  m_payment_to_fid;
		date::sys_days				m_statement_date;
	};


}