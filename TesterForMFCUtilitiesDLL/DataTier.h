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
	class Category
	{
	public:
		// primary key:
		std::string					m_name;
		bool						m_real_expense_or_income;
	};

	class Account
	{
	public:
		// primary key
		std::string m_number;
		Coin		m_currency;
	};

	class StatementLine
	{
	public:
	// id (primary key)
		unsigned long				m_id;
		std::string					m_account;	// credit card # or bank account #
		date::sys_days				m_lineDate;
		std::string					m_concept;
		std::wstring				m_wide;
		Colones						m_amountInLocal;
		Dolares						m_amountInDollars;

#if 0
		std::shared_ptr<Category>	m_category;
		bool						m_enabled;
		bool						m_isDirty = false;
		std::string					m_owner;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A category dependencies. </summary>
		///
		/// <remarks>	Juan Dent, 31/10/2017. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool						m_bCategoryDependsOnAccount = false;	// otherwise depends only on concept
		bool						m_bApplyOnceOnly = false;
		std::string					m_description;
#endif
	};


}