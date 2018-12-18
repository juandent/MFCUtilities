#pragma once
#include "CSVFile.h"
#include "../Model/Model.ORM.Definitions.h"

namespace Controller
{
	class LoadCompoundDocIntoDB
	{
		CSVFile m_file_reader;
		Model::Statement m_statement;
		Model::Nullable::Type<Model::Account> m_account;
		Model::Nullable::Type<Model::Person> m_owner;
		Model::Nullable::Type<Model::Concept> m_concept;
		Model::Nullable::Type<Model::Category> m_category;
		Model::Nullable::Type<Model::StatementLine> m_statement_line;


		enum Columns
		{
			StatementsDate,
			Account,
			LineDate,
			Concept,
			AmountInLocal,
			AmountInDollars,
			IsEnabled,
			Category,
			Description,
			Owner
		};
		std::vector<std::string> extractOwnerNameParts(const std::string& cs);
	public:
		explicit LoadCompoundDocIntoDB(const std::string& file_name);
		~LoadCompoundDocIntoDB();

		void getConcept(size_t row, size_t col);
		void getCategory(size_t row, size_t col);
		void getStatementLine(size_t row);
		void DoLoadIntoDB();
		
		void getOwner(size_t row, size_t col);
		void getAccount(size_t row, size_t col);

	};

}
