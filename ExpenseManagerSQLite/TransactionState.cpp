#include "stdafx.h"
#include "TransactionState.h"



TransactionState::TransactionState(TransaccionDlg& dlg)
	: dlg{dlg}
{
	m_trans = dlg.m_transaccionLB.current();
	m_other_account = dlg.m_other_accountCB.current();
	m_statement = dlg.m_statementCB.current();
	m_own_account = dlg.m_own_accountCB.current();
	m_categoria = dlg.m_categoriaCB.current();
	m_concepto = dlg.m_conceptoCB.current();
}

void TransactionState::Restore()
{
	if( m_trans)
	{
		dlg.m_transaccionLB.select(m_trans->id_transaccion);
	}
	if(m_other_account)
	{
		dlg.m_other_accountCB.select(m_other_account->id_account);
	}
	if(m_statement)
	{
		dlg.m_statementCB.select(m_statement->id_statement);
	}
	if(m_concepto)
	{
		dlg.m_conceptoCB.select(m_concepto->id_concepto);
	}
	if(m_own_account)
	{
		dlg.m_own_accountCB.select(m_own_account->id_account);
	}
	if( m_categoria)
	{
		dlg.m_categoriaCB.select(m_categoria->id_categoria);
	}
}
