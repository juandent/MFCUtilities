#include "stdafx.h"
#include "Controller.LoadCompoundDocIntoDB.h"
#include "../Model/Model.ORM.Definitions.h"
#include "CSVFile.h"
#include <filesystem>


using namespace std::string_literals;

namespace Controller
{
	using namespace Model;
	using namespace sqlite_orm;
	using namespace std;
	namespace fs = std::filesystem;


	LoadCompoundDocIntoDB::LoadCompoundDocIntoDB(const std::string & file_name)
	{
		m_file_reader.load(file_name);
		m_statement.m_filePath = file_name;
		auto complete_path = fs::path{ file_name };
		auto only_file_name = complete_path.filename();
		m_statement.m_fileName = only_file_name.string();
	}

	LoadCompoundDocIntoDB::~LoadCompoundDocIntoDB()
	{
	}

	void LoadCompoundDocIntoDB::getConcept(size_t row, size_t col)
	{
		auto str = m_file_reader.getString({ row,col });
		m_concept = ORM::storage.get_no_throw<Model::Concept>(str);
		if (m_concept == nullptr)
		{
			m_concept = std::make_shared<Model::Concept>();
			m_concept->m_concept_id = str;
			m_concept->m_account_payment_fid = nullptr; // Nullable::make_nullable(m_account->m_number_id);
			m_concept->m_always = true;
			m_concept->m_is_regex = false;
			ORM::storage.replace(*m_concept);
		}
	}

	void LoadCompoundDocIntoDB::getCategory(size_t row, size_t col)
	{
		auto str = m_file_reader.getString({ row, col });
		m_category = ORM::storage.get_no_throw<Model::Category>(str);
		if (m_category == nullptr)
		{
			m_category = std::make_shared<Model::Category>();
			m_category->m_name_id = str;
			// TODO: make a list of categories for which the next is true
			m_category->m_real_expense_or_income = false;
			ORM::storage.replace(*m_category);
		}
	}

	void LoadCompoundDocIntoDB::getStatementLine(size_t row)
	{
		auto line_date = m_file_reader.getDate({ row, Columns::LineDate });
		auto statement_date = m_file_reader.getDate({ row, Columns::StatementsDate });
		Colones amount_local = m_file_reader.getMoney({ row, Columns::AmountInLocal });
		Dolares amount_dollars = m_file_reader.getMoney({ row, Columns::AmountInDollars });
#if 0
		auto matching = ORM::storage.get_all<StatementLine>(
			where(c(&StatementLine::m_amountInLocal) == amount_local &&
				c(&StatementLine::m_amountInDollars) == amount_dollars &&
				c(&StatementLine::m_amountInLocal) == amount_local &&
				c(&StatementLine::m_concept_fid) == m_concept->m_concept_id &&
				c(&StatementLine::m_lineDate) == line_date &&
				c(&StatementLine::m_statement_date) == statement_date &&
				c(&StatementLine::m_belongs_to_account_fid) == m_account->m_number_id));

		//std::vector<StatementLine>matching;
		if (matching.size() > 0)
		{
			string str = "Seems statementline is duplicate for account "s + m_account->m_number_id
				+ " and line date "s + JD::to_string(line_date);

			throw std::exception(str.c_str());
		}
#endif
		m_statement_line = std::make_shared<StatementLine>();
		m_statement_line->m_belongs_to_account_fid = m_account->m_number_id;
		m_statement_line->m_amountInDollars = amount_dollars;
		m_statement_line->m_amountInLocal = amount_local;
		m_statement_line->m_category_fid = Nullable::make_nullable(m_category->m_name_id);
		m_statement_line->m_concept_fid = m_concept->m_concept_id;
		m_statement_line->m_details = m_file_reader.getString({ row, Columns::Description });
		m_statement_line->m_enabled = m_file_reader.getBool({ row, Columns::IsEnabled });
		m_statement_line->m_lineDate = line_date;
		m_statement_line->m_statement_date = statement_date;
		m_statement_line->m_id = ORM::storage.insert(*m_statement_line);
		m_statement_line->AddResponsible(*m_owner);

	}

	void LoadCompoundDocIntoDB::DoLoadIntoDB()
	{
		ORM::storage.begin_transaction();

		m_statement.m_statementDate = m_file_reader.getDate({ 1,0 });
		auto matching = ORM::storage.get_all<Statement>(
			where(c(&Statement::m_statementDate) == m_statement.m_statementDate));
		if (matching.size() > 0)
		{
			string msg = "Duplicate compound statement for date "s + JD::to_string(m_statement.m_statementDate);
			throw std::exception(msg.c_str());
		}

		ORM::storage.replace(m_statement);

		for (size_t row = 1; row < m_file_reader.getRowCount(); ++row)
		{
			getOwner(row, Columns::Owner);
			getAccount(row, Columns::Account);
			getCategory(row, Columns::Category);
			getConcept(row, Columns::Concept);
			getStatementLine(row);
		}
		ORM::storage.commit();
	}



	void LoadCompoundDocIntoDB::getOwner(size_t row, size_t col)
	{
		auto cs = getOwnerContentsFor(row);
		std::vector<std::string> name_parts = extractOwnerNameParts(cs);

		string first_name;
		string last_name;
		switch (name_parts.size())
		{
		case 2:
			first_name = name_parts[0];
			last_name = name_parts[1];
			break;
		case 3:
			first_name = name_parts[0];
			last_name = name_parts[1] + " " + name_parts[2];
			break;
		default:
			throw std::exception("Owner name has unexpected number of parts");
		}

		auto matching_persons = ORM::storage.get_all<Model::Person>(where(
			c(&Person::m_first_name) == first_name &&
			c(&Person::m_last_name) == last_name
			));
		if (matching_persons.size() == 0)
		{
			m_owner = std::make_shared<Model::Person>();
			m_owner->m_first_name = first_name;
			m_owner->m_last_name = last_name;
			m_owner->m_id = ORM::storage.insert(*m_owner);
		}
		else
		{
			if (matching_persons.size() > 1)
			{
				throw std::exception("duplicate persons");
			}
			auto existing_person = matching_persons.front();
			m_owner = Nullable::make_nullable(existing_person);
		}
	}

	std::string LoadCompoundDocIntoDB::getAccountDescription(size_t row)
	{
		auto str = getOwnerContentsFor(row);
		auto pos = str.find('-');
		if( pos != string::npos)
		{
			return str.substr(pos + 1);
		}
		else
			return "";
	}

	void LoadCompoundDocIntoDB::getAccount(size_t row, size_t col)
	{
		auto pk = m_file_reader.getString({ row,col });
		m_account = ORM::storage.get_no_throw<Model::Account>(pk);
		if (m_account == nullptr)	// then create one
		{
			m_account = std::make_shared<Model::Account>();
			m_account->AssignPK(pk);
			Dolares dollar_amount = m_file_reader.getMoney({ row, Columns::AmountInDollars });
			m_account->SetCurrencyType(dollar_amount);
			m_account->m_owner_fid = m_owner->m_id;
			m_account->m_description = getAccountDescription(row); //  m_file_reader.getString({ row, Columns::Owner });
			ORM::storage.replace(*m_account);
		}
	}


	std::string LoadCompoundDocIntoDB::getOwnerContentsFor(size_t row)
	{
		try
		{
			auto theRow = m_file_reader.getRow(row);
			if (Columns::Owner >= theRow.size())
			{
				throw std::exception("column number out of bounds");
			}
			auto cs = m_file_reader.getString({ row,Columns::Owner });
			return cs;
			//name_parts = extractOwnerNameParts(cs);
		}
		catch (std::exception& ex)
		{
			auto wh = ex.what();

			return "Unknown owner";
			// missing owner - for now just report that
		}
	}

	std::vector<std::string> LoadCompoundDocIntoDB::extractOwnerNameParts(const std::string & cs)
	{
		auto dash_pos = cs.find('-');
		std::string full_name;
		if (dash_pos != std::string::npos)
		{
			full_name = cs.substr(0, --dash_pos);
		}
		else
		{
			full_name = cs;
		}

		vector<string> name_parts;

		regex sep("[ \t\n]+");  // separated by spaces

		sregex_token_iterator p(full_name.cbegin(), full_name.cend(),  // sequence
			sep,														// separator
			-1);														// -1: values between separators

		sregex_token_iterator e;
		for (; p != e; ++p) {
			auto str = p->str();
			name_parts.push_back(str);
		}
		return name_parts;
	}
}
