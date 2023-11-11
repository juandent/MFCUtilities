#pragma once


#include "TransaccionDlg.h"

class TransactionState
{
	void Restore();
	TransaccionDlg& dlg;

	std::optional<Transaccion> m_trans;
	std::optional<Concepto> m_concepto;
	std::optional<Account>  m_own_account;
	std::optional<Account>  m_other_account;
	std::optional<Statement> m_statement;
	std::optional<Categoria> m_categoria;

public:
	TransactionState(TransaccionDlg& dlg);
	~TransactionState() { Restore(); }

};
