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

		std::string getOwnerContentsFor(size_t row);

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

		std::string getAccountDescription(size_t row);
		void getAccount(size_t row, size_t col);

		struct Selects
		{
			static auto& getStatementLineFlat()
			{
				using namespace sqlite_orm;
				using namespace Model;

				static auto statement_line_flattened = ORM::storage
					.select(columns(&StatementLine::m_statement_date,
						&StatementLine::m_belongs_to_account_fid,
						&StatementLine::m_lineDate,
						&StatementLine::m_concept_fid,
						&StatementLine::m_amountInLocal,
						&StatementLine::m_amountInDollars,
						&StatementLine::m_enabled,
						&StatementLine::m_category_fid));
				return statement_line_flattened;
			}
			inline static std::vector<std::string> headers{ "STMT DATE", "ACCOUNT", "LINE DATE", "CONCEPT", "AMT LOCAL","AMT DOLLARS", "ENABLED?","CATEGORY" };
		};

	};

}
