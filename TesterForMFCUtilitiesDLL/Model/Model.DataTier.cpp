#include "stdafx.h"
#include "Model.DataTier.h"
#include "Model.ORM.Definitions.h"

using namespace Model;
using namespace ORM;


Person Model::Account::getOwner()
{
	auto person = ORM::Storage::getStorage().get<Person>(m_owner_fid);
	return person;
}



Category Model::Concept::getCategory()
{
	auto cat = ORM::Storage::getStorage().get<Category>(m_category_name_fid);
	return cat;
}

Nullable::Type<Account> Model::Concept::getAccount()
{
	return Nullable::getFromFKs<Account>(m_account_payment_fid);
}

Person Model::Responsible::getPerson()
{
	auto person = ORM::Storage::getStorage().get<Person>(m_person_fid);
	return person;
}

void Model::LineResponsibility::AssignFK(const StatementLine & stmtLine)
{
	m_statement_fid = stmtLine.m_id;
}

void Model::LineResponsibility::AssignFK(const Responsible & resp)
{
	m_responsible_fid = resp.m_id;
}

StatementLine Model::LineResponsibility::getStatementLine()
{
	auto stmt = ORM::Storage::getStorage().get<StatementLine>(m_statement_fid);
	return stmt;
}

Responsible Model::LineResponsibility::getResponsible()
{
	auto stmt = ORM::Storage::getStorage().get<Responsible>(m_responsible_fid);
	return stmt;
}

void Model::Statement::AssignFK(const Account & act)
{
	m_account_fid = act.m_number_id;
}

Account Model::Statement::getAccount()
{
	auto act = ORM::Storage::getStorage().get<Account>(m_account_fid);
	return act;
}

void Model::StatementLine::AssignBelongingFK(const Account & act)
{
	m_account_fid = act.m_number_id;
}

void Model::StatementLine::AssignFK(const Concept & con)
{
	m_concept_fid = con.m_concept_id;
}

void Model::StatementLine::AssignFK(const Category & cat)
{
	auto catid = Nullable::make_nullable(cat.m_name_id);
	m_category_fid = catid;
}

void Model::StatementLine::AssignPaymentFK(const Account & act)
{
	auto actId = Nullable::make_nullable(act.m_number_id);
	m_payment_to_fid = actId;
}

Account Model::StatementLine::getAccountBelonging()
{
	auto act = ORM::Storage::getStorage().get<Account>(m_account_fid);
	return act;
}

Concept Model::StatementLine::getConcept()
{
	auto conc = ORM::Storage::getStorage().get<Concept>(m_concept_fid);
	return conc;
}

Nullable::Type<Category> Model::StatementLine::getCategory()
{
	return Nullable::getFromFKs<Category>(m_category_fid);
}
