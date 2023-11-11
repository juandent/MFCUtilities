#pragma once
#include "TablePool.h"
#include "Data_Tier.h"

struct Translation;

class StatementLineAdapter
{
	TablePool<Statement, date::sys_days> m_statement_pool;
	TablePool<Concepto, std::string>     m_concepto_pool;
	TablePool<Categoria, std::string>    m_categoria_pool;
	TablePool<Transaccion, int>			 m_transaccion_pool;
	
public:
    StatementLineAdapter();

	void adapt(const Translation& t);

	template<typename Table, typename SearchType>
	std::shared_ptr<Table> get(const Translation& t);
	
};

template <typename Table, typename SearchType>
std::shared_ptr<Table> StatementLineAdapter::get(const Translation& t)
{
	
}
