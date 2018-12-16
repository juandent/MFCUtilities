#include "stdafx.h"
#include "Controller.LoadCompoundDocIntoDB.h"
#include "../Model/Model.ORM.Definitions.h"
#include "CSVFile.h"
#include <filesystem>


namespace Controller
{
	using namespace Model;
	using namespace sqlite_orm;
	using namespace std;
	namespace fs = std::experimental::filesystem;


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

	void LoadCompoundDocIntoDB::DoLoadIntoDB()
	{
		m_statement.m_statementDate = m_file_reader.getDate({ 1,0 });

		for(size_t row=1; row < m_file_reader.getRowCount(); ++row)
		{
			auto act = m_file_reader.getString({ row, Columns::Account });
			auto account = getAccount(row, act);

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

	Model::Nullable::Type<Model::Person> LoadCompoundDocIntoDB::getOwner(const std::vector<std::string>& name_parts)
	{
		string first_name;
		string last_name;
		switch(name_parts.size())
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
		if( matching_persons.size() == 0)
		{
			auto person = std::make_shared<Model::Person>();
			person->m_first_name = first_name;
			person->m_last_name = last_name;
			person->m_id = ORM::storage.insert(*person);
			return person;
		}
		else
		{
			if( matching_persons.size() > 1)
			{
				throw std::exception("too many matches in Person");
			}
			auto existing_person = matching_persons.front();
			return Nullable::make_nullable(existing_person);
		}
	}

	Model::Nullable::Type<Model::Account> LoadCompoundDocIntoDB::getAccount(size_t row, const std::string & pk)
	{
		auto account_in_storage = ORM::storage.get_no_throw<Model::Account>(pk);
		if (account_in_storage == nullptr)	// then create one
		{
			account_in_storage.swap(std::make_shared<Model::Account>());
			account_in_storage->AssignPK(pk);
			auto dollar_amount = m_file_reader.getMoney({ row, Columns::AmountInDollars });
			account_in_storage->SetCurrencyType(dollar_amount);
			auto owner_str = m_file_reader.getText( row, Columns::Owner );
			auto owner_name_parts = extractOwnerNameParts(owner_str);
			auto owner = getOwner(owner_name_parts);
			account_in_storage->m_owner_fid = owner->m_id;
			account_in_storage->m_description = m_file_reader.getText(row, Columns::Description);
			ORM::storage.replace(*account_in_storage);
		}
		return account_in_storage;
	}

}
