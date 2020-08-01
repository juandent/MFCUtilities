#pragma once

#include "CategoryDlg.h"
#include "CuentaDlg.h"
#include "Data_Tier.h"
#include "ConceptosDlg.h"
#include "StatementDlg.h"
#include "TransaccionDlg.h"

class CompoundStatementLine
{
	std::optional<Transaccion> m_trans;
	std::optional<Account>     m_own_account;
	std::optional<Account>     m_other_acccount;
	std::optional<Concepto>    m_concepto;
	std::optional<Statement>   m_statement;
	std::optional<Categoria>   m_categoria;

	auto insert_own_account()
	{
		// COLS PRESENT: number
		// COLS ABSENT : is_tarjeta
		// FKS:  banco.fkey, account_owner.fkey

		CuentaDlg dlg;
		dlg.m_numero_val = m_own_account_number;
		dlg.DoModal();		
		return dlg.m_account;
	}
	auto insert_concepto()
	{
		ConceptosDlg dlg;
		dlg.setDiscriminator( m_concepto_name);
		dlg.DoModal();
		m_concepto = dlg.getCompleteObject();
		return m_concepto;
	}
	auto insert_statement()
	{
		StatementDlg dlg;
		dlg.setDiscriminator(m_statement_date);
		dlg.DoModal();
		return dlg.getCompleteObject();
	}
	auto insert_category()
	{
		CategoryDlg dlg;
		dlg.setDiscriminator(m_category_name);
		dlg.DoModal();
		return dlg.getCompleteObject();
	}
	
	double m_amount_colones{};
	double m_amount_dolares{};
	std::string m_own_account_number;
	std::string m_concepto_name;
	date::sys_days m_line_date;
	date::sys_days m_statement_date;
	std::string m_description;
	std::string m_category_name;

public:
	auto set_own_account(const std::string& number)
	{
		m_own_account_number = number;
		
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto clause = c(&Account::number) == m_own_account_number;
		auto vec = storage.select(columns(&Account::id_account, &Account::number), where(clause));

		if( vec.size() == 0)
		{
			m_own_account = insert_own_account();
		}
		else
		{
			auto id = std::get<0>(vec[0]);
			m_own_account = storage.get<Account>(id);
		}
		return m_own_account;
	}
	auto set_concepto(const std::string& name )
	{
		m_concepto_name = name;

		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto clause = c(&Concepto::name) == m_concepto_name;
		auto vec = storage.select(columns(&Concepto::id_concepto, &Concepto::name), where(clause));

		if (vec.size() == 0)
		{
			m_concepto = insert_concepto();
		}
		else
		{
			auto id = std::get<0>(vec[0]);
			m_concepto = storage.get<Concepto>(id);
		}
		return m_concepto;
	}
	auto setStatement( date::sys_days fecha)
	{
		m_statement_date = fecha;
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto clause = c(&Statement::date) == m_statement_date;
		auto vec = storage.select(columns(&Statement::id_statement, &Statement::date), where(clause));

		if (vec.size() == 0)
		{
			m_statement = insert_statement();
		}
		else
		{
			auto id = std::get<0>(vec[0]);
			m_statement = storage.get<Statement>(id);
		}
		return m_statement;
	}
	auto set_category( const std::string& category_name)
	{
		m_category_name = category_name;
		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto clause = c(&Categoria::name) == m_category_name;
		auto vec = storage.select(columns(&Categoria::id_categoria, &Categoria::name), where(clause));

		if (vec.size() == 0)
		{
			m_categoria = insert_category();
		}
		else
		{
			auto id = std::get<0>(vec[0]);
			m_categoria = storage.get<Categoria>(id);
		}
		return m_categoria;
	}
	std::optional<Transaccion> set_transaction(double colones, double dolares, date::sys_days line_date, const std::string& description )
	{
		if( ! m_concepto || ! m_statement || ! m_categoria || ! m_own_account )
		{
			return std::nullopt;
		}
		Transaccion trans;
		m_trans = trans;

		m_trans->fkey_concepto = m_concepto->id_concepto;
		m_trans->fkey_account_own = m_own_account->id_account;
		m_trans->fkey_category = m_categoria->id_categoria;
		m_trans->fkey_statement = m_statement->id_statement;
		m_trans->amount_colones = colones;
		m_trans->amount_dolares = dolares;
		m_trans->descripcion = description;
		m_trans->line_date = line_date;

		
		TransaccionDlg dlg;
		dlg.setDiscriminator(m_trans);
		dlg.DoModal();
		m_trans = dlg.getCompleteObject();
		return m_trans;
	}
	
};
