#pragma once
#include "CSVFile.h"
#include "../Model/Model.ORM.Definitions.h"

namespace Controller
{
	class LoadCompoundDocIntoDB
	{
		CSVFile m_file_reader;
		Model::Statement m_statement;

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

	public:
		explicit LoadCompoundDocIntoDB(const std::string& file_name);
		~LoadCompoundDocIntoDB();
		void DoLoadIntoDB();
		std::vector<std::string> extractOwnerNameParts(const std::string& cs);
		Model::Nullable::Type<Model::Person> getOwner( const std::vector<std::string>& name_parts);
		Model::Nullable::Type<Model::Account> getAccount(size_t row, const std::string& pk);

	};

}
