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
#define PrimaryKey
#define ForeignKey(Table)

	struct Category_Rec
	{
		PrimaryKey std::string					m_name_id;
		bool						m_real_expense_or_income;
	};

	struct Person_Rec					// Leslie, Juan Jr, Juan Sr, Other
	{
		PrimaryKey int m_id;
		std::string m_first_name;
		std::string m_last_name;
	};

	//struct Owner_Rec
	//{
	//	PrimaryKey int m_id;
	//	int m_person_fid;
	//};

	struct Account_Rec
	{
		PrimaryKey std::string m_number_id;
		ForeignKey(Person_Rec) int m_owner_fid;
		Coin		m_currency;
	};

	struct Concept_Rec
	{
		PrimaryKey std::string m_concept_id;
		ForeignKey(Category_Rec)  std::string m_category_name_fid;
		bool m_always;	// always apply this mapping rather than just suggesting
	};

	struct RegexConcept_Rec
	{
		PrimaryKey std::string m_regex_concept_id;
		ForeignKey(Category_Rec)  std::string m_category_name_fid;
		bool m_always;	// always apply this mapping rather than just suggesting
	};

	struct Responsible_Rec
	{
		PrimaryKey int m_id;
		ForeignKey(Person_Rec) int m_person_fid;
		double m_percentage;	// degree of responsibility
	};

	// N:M
	struct LineResponsibility_Rec
	{
		ForeignKey(StatementLine_Rec) unsigned long m_statement_fid;
		ForeignKey(Responsible_Rec)   int m_responsible_fid;
	};

	struct StatementLine_Rec
	{
		PrimaryKey unsigned long	m_id;
		ForeignKey(Account_Rec) std::string		m_account_fid;
		date::sys_days				m_lineDate;
		ForeignKey(Concept_Rec) std::string		m_concept_fid;
		Colones						m_amountInLocal;
		Dolares						m_amountInDollars;

		ForeignKey(Category_Rec) std::shared_ptr<std::string>	m_category_fid;
		bool						m_enabled;
		std::string					m_details;
	};


}