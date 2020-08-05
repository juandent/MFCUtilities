#pragma once

#include "CategoryDlg.h"
#include "CuentaDlg.h"
#include "Data_Tier.h"
#include "ConceptosDlg.h"
#include "StatementDlg.h"
#include "TransaccionDlg.h"
#include "ConceptsAndAccounts.h"

class CompoundStatementLine
{
	//inline static std::map<int, std::optional<Transaccion>> s_transaccion_map;
	
	//int m_row;
	ConceptsAndAccounts*		m_table_source;
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
		dlg.set_discriminator(m_own_account_number);
		// dlg.m_numero_val = m_own_account_number;
		dlg.DoModal();		
		return dlg.getCompleteObject();
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
	auto set_own_account( ConceptsAndAccounts::StringColumn_mem fn)
	{
		m_own_account_number = (m_table_source->*fn)(m_table_source->Row());

		using namespace sqlite_orm;
		auto& storage = Storage::getStorage();

		auto clause = c(&Account::number) == m_own_account_number;
		auto vec = storage.select(columns(&Account::id_account, &Account::number), where(clause));

		if (vec.size() == 0)
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
	
public:
	CompoundStatementLine(ConceptsAndAccounts* table_source) : m_table_source(table_source) {}

#if 0
	auto set_own_account( std::string &ConceptAndAccounts::f(int), ConceptAndAccounts* obj)
	{
		auto res = obj.*f(m_row);
		return set_own_account(res);
	}
#endif
	
	auto set_concepto(ConceptsAndAccounts::StringColumn_mem fn )
	{
		m_concepto_name = (m_table_source->*fn)(m_table_source->Row());

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
	auto setStatement(ConceptsAndAccounts::SysDaysColumn_mem fn )
	{
		m_statement_date = (m_table_source->*fn)(m_table_source->Row());
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
	auto set_category(ConceptsAndAccounts::StringColumn_mem fn)
	{
		m_category_name = (m_table_source->*fn)(m_table_source->Row());
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
	std::optional<Transaccion> set_transaction(ConceptsAndAccounts::DoubleColumn_mem colones_fn, ConceptsAndAccounts::DoubleColumn_mem  dolares_fn, 
		ConceptsAndAccounts::SysDaysColumn_mem line_date_fn, ConceptsAndAccounts::StringColumn_mem description_fn )
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
		m_trans->amount_colones = (m_table_source->*colones_fn)(m_table_source->Row());
		m_trans->amount_dolares = (m_table_source->*dolares_fn)(m_table_source->Row());
		m_trans->descripcion = (m_table_source->*description_fn)(m_table_source->Row());
		m_trans->line_date = (m_table_source->*line_date_fn)(m_table_source->Row());
		m_trans->descripcion = (m_table_source->*description_fn)(m_table_source->Row());
#define MODIFY_SCHEMA
#ifdef MODIFY_SCHEMA
		m_trans->row = m_table_source->Row();
#endif
		m_trans->id_transaccion = -1;
		
		TransaccionDlg dlg;
		dlg.setDiscriminator(m_trans);
		dlg.autoexec();
		auto ret = dlg.DoModal();
		m_trans = dlg.getCompleteObject();
		return m_trans;
	}
	
};
