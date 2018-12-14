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

	Model::Nullable::Type<Model::Account> LoadCompoundDocIntoDB::getAccount(size_t row, const std::string & pk)
	{
		auto account_in_storage = ORM::storage.get_no_throw<Model::Account>(pk);
		if (account_in_storage == nullptr)	// then create one
		{
			account_in_storage.swap(std::make_shared<Model::Account>());
			account_in_storage->AssignPK(pk);
			auto dollar_amount = m_file_reader.getMoney({ row, Columns::AmountInDollars });
			account_in_storage->SetCurrencyType(dollar_amount);
			account_in_storage->m_
			
			ORM::storage.replace(*account_in_storage);
		}
		return account_in_storage;
		return Model::Account();
	}

	void LoadCompoundDocIntoDB::SetCurrencyType(Money dollar_amount)
	{
	}

}