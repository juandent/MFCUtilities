#include "stdafx.h"
#include "Model.DataTier.h"
#include "Model.ORM.Definitions.h"

using namespace Model;
using namespace ORM;
using namespace sqlite_orm;


Person Model::Account::getOwner()
{
	auto person = ORM::storage.get<Person>(m_owner_fid);
	return person;
}


//Nullable::Type<Category> Model::Concept::getCategory()
//{
//	return Nullable::getFromFKs<Category>(m_category_name_fid);
//}

Nullable::Type<Account> Model::Concept::getAccount()
{
	return Nullable::getFromFKs<Account>(m_account_payment_fid);
}

Person Model::Responsible::getPerson()
{
	auto person = ORM::storage.get<Person>(m_person_fid);
	return person;
}

void Model::LineResponsibility::AssignFK(const StatementLine & stmtLine)
{
	m_statement_line_fid = stmtLine.m_id;
}

void Model::LineResponsibility::AssignFK(const Responsible & resp)
{
	m_responsible_fid = resp.m_id;
}

StatementLine Model::LineResponsibility::getStatementLine()
{
	auto stmt = ORM::storage.get<StatementLine>(m_statement_line_fid);
	return stmt;
}

Responsible Model::LineResponsibility::getResponsible()
{
	auto stmt = ORM::storage.get<Responsible>(m_responsible_fid);
	return stmt;
}

void Model::AccountStatement::AssignFK(const Account & act)
{
	m_account_fid = act.m_number_id;
}

Account Model::AccountStatement::getAccount()
{
	auto act = ORM::storage.get<Account>(m_account_fid);
	return act;
}

void Model::StatementLine::AssignBelongingFK(const Account & act)
{
	this->m_belongs_to_account_fid = act.m_number_id;
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

void Model::StatementLine::AssignReferingFK(const Account & act)
{
	auto actId = Nullable::make_nullable(act.m_number_id);
	this->m_refers_to_account_fid = actId;
}

Account Model::StatementLine::getAccountBelonging()
{
	auto act = ORM::storage.get<Account>(this->m_belongs_to_account_fid);
	return act;
}

Nullable::Type<Account> Model::StatementLine::getAccountRefering()
{
	auto act = ORM::storage.get<Account>(this->m_refers_to_account_fid);
	return Nullable::make_nullable(act);
}

Concept Model::StatementLine::getConcept()
{
	auto conc = ORM::storage.get<Concept>(m_concept_fid);
	return conc;
}

Nullable::Type<Category> Model::StatementLine::getCategory()
{
	return Nullable::getFromFKs<Category>(m_category_fid);
}

Statement Model::StatementLine::getStatement()
{
	try
	{
		auto conc = ORM::storage.get<Statement>(this->m_statement_date);
		return conc;
	}
	catch (std::exception& ex)
	{
		auto w = ex.what();
		throw;
	}
}

std::vector<Responsible> Model::StatementLine::getResponsibles()
{
	std::vector<Responsible> r_vec;
	auto vec = ORM::storage.get_all<LineResponsibility>(where(c(&LineResponsibility::m_statement_line_fid) == m_id));
	for (auto& lr : vec)
	{
		r_vec.push_back(lr.getResponsible());
	}
	return r_vec;
}

void Model::StatementLine::AddResponsible(const Person & person, double percentage)
{
	// Responsibles
	Responsible resp{ -1, person.m_id, percentage};

	resp.m_id = ORM::storage.insert(resp);

	// LineResponsibility
	LineResponsibility lresp{};
	lresp.AssignFK(*this);
	lresp.AssignFK(resp);

	ORM::storage.insert(lresp);
}

std::vector<StatementLine> Model::Category::getStatementLines()
{
	auto vec = ORM::storage.get_all<StatementLine>(where(c(&StatementLine::m_category_fid) == m_name_id));
	return vec;
}
