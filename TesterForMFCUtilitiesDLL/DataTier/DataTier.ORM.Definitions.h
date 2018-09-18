#pragma once

#include "DataTier.h"

namespace ORM
{ 

	class ORM_Central
	{
	public:
		static auto& refresh_and_get_schema();
		void fill_db();
		void remove_all_from_database();

	};

	inline 	auto& ORM_Central::refresh_and_get_schema()
	{
		using namespace sqlite_orm;
		using namespace date;
		using namespace std;
		using namespace DataTier;

		static int flag = 0;
		static auto storage = make_storage("statements.sqlite",

			make_table("PERSONS"s,
				make_column("id",
					&Person::m_id,
					primary_key()),
				make_column("first_name",
					&Person::m_first_name),
				make_column("last_name",
					&Person::m_last_name),
				make_column("company_name",
					&Person::m_company_name)),

			make_table("RESPONSIBLES"s,
				make_column("id",
					&Responsible::m_id,
					primary_key()),
				make_column("person",
					&Responsible::m_person_fid),
				make_column("percentage",
					&Responsible::m_percentage),
				foreign_key(&Responsible::m_person_fid).references(&Person::m_id)
			),

			make_table("CATEGORIES"s,
				make_column("name",
					&Category::m_name_id,
					primary_key()),
				make_column("is_real",
					&Category::m_real_expense_or_income)),

			make_table("CONCEPTS"s,
				make_column("id",
					&Concept::m_concept_id,
					primary_key()),
				make_column("category",
					&Concept::m_category_name_fid),
				make_column("account_payment",
					&Concept::m_account_payment_fid),
				make_column("always",
					&Concept::m_always),
				make_column("is_regex",
					&Concept::m_is_regex),
				foreign_key(&Concept::m_category_name_fid).references(&Category::m_name_id),
				foreign_key(&Concept::m_account_payment_fid).references(&Account::m_number_id)
			),
			make_table("STATEMENTS"s,
				make_column("file_name",
					&Statement::m_fileName),
				make_column("file_path",
					&Statement::m_filePath),
				make_column("statement_date",
					&Statement::m_statementDate),
				make_column("account",
					&Statement::m_account_fid),
				primary_key(&Statement::m_account_fid, &Statement::m_statementDate),
				foreign_key(&Statement::m_account_fid).references(&Account::m_number_id)
			),

			make_table("STATEMENTLINES"s,
				make_column("id",
					&StatementLine::m_id,
					primary_key(),
					autoincrement()),
				make_column("account",
					&StatementLine::m_account_fid),
				make_column("statement_date",
					&StatementLine::m_statement_date),
				make_column("line_date",
					&StatementLine::m_lineDate),
				make_column("concept",
					&StatementLine::m_concept_fid),
				make_column("colones"s,
					&StatementLine::m_amountInLocal),
				make_column("dolares"s,
					&StatementLine::m_amountInDollars),
				make_column("category"s,
					&StatementLine::m_category_fid),
				make_column("enabled",
					&StatementLine::m_enabled),
				make_column("details",
					&StatementLine::m_details),
				make_column("payment_to",
					&StatementLine::m_payment_to_fid),
				foreign_key(&StatementLine::m_account_fid).references(&Account::m_number_id),
				foreign_key(&StatementLine::m_category_fid).references(&Category::m_name_id),
				foreign_key(&StatementLine::m_concept_fid).references(&Concept::m_concept_id),
				foreign_key(&StatementLine::m_payment_to_fid).references(&Account::m_number_id)
				//foreign_key(&StatementLine::m_account_fid, &StatementLine::m_statement_date).references(&Statement::m_account_fid, &Statement::m_statement_date))
			),

			make_table("LINE_RESPONSABILITIES"s,
				make_column("statement_id",
					&LineResponsibility::m_statement_fid),
				make_column("responsible_id",
					&LineResponsibility::m_responsible_fid),
				foreign_key(&LineResponsibility::m_statement_fid).references(&StatementLine::m_id),
				foreign_key(&LineResponsibility::m_responsible_fid).references(&Responsible::m_id))
  ,

				make_table("ACCOUNTS"s,
					make_column("number",
						&Account::m_number_id,
						primary_key()),
					make_column("owner",
						&Account::m_owner_fid),
					make_column("currency",
						&Account::m_currency),
					make_column("description",
						&Account::m_description),
					make_column("type",
						&Account::m_type),
					make_column("cuenta_cliente",
						&Account::m_cuenta_cliente),
					foreign_key(&Account::m_owner_fid).references(&Person::m_id))
				);

		if (flag == 0)
		{
			flag = 1;
			storage.sync_schema();
		}
		return storage;
	}

}
