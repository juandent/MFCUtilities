#pragma once

#include "DataTier.h"

namespace ORM
{
	inline auto& getStorage()
	{
		using namespace sqlite_orm;
		using namespace date;
		using namespace std;
		using namespace DataTier;

		static int flag = 0;
		static auto storage = make_storage("statements.sqlite",
			make_table("CATEGORIES"s,
				make_column("name",
					&Category_Rec::m_name_id,
					primary_key()),
				make_column("is_real",
					&Category_Rec::m_real_expense_or_income)),
			make_table("PERSONS"s,
				make_column("id",
					&Person_Rec::m_id,
					primary_key()),
				make_column("first_name",
					&Person_Rec::m_first_name),
				make_column("last_name",
					&Person_Rec::m_last_name),
				make_column("company_name",
					&Person_Rec::m_company_name)),
			make_table("ACCOUNTS"s,
				make_column("number",
					&Account_Rec::m_number_id,
					primary_key()),
				make_column("owner",
					&Account_Rec::m_owner_fid),
				make_column("currency",
					&Account_Rec::m_currency)),
			make_table("CONCEPTS"s,
				make_column("id",
					&Concept_Rec::m_concept_id,
					primary_key()),
				make_column("category",
					&Concept_Rec::m_category_name_fid),
				make_column("always",
					&Concept_Rec::m_always)
			),
			make_table("REGEXCONCEPTS"s,
				make_column("id",
					&RegexConcept_Rec::m_regex_concept_id,
					primary_key()),
				make_column("category",
					&RegexConcept_Rec::m_category_name_fid),
				make_column("always",
					&RegexConcept_Rec::m_always)
			),
			make_table("RESPONSIBLES"s,
				make_column("id",
					&Responsible_Rec::m_id,
					primary_key()),
				make_column("person",
					&Responsible_Rec::m_person_fid),
				make_column("percentage",
					&Responsible_Rec::m_percentage),
				foreign_key(&Responsible_Rec::m_person_fid).references(&Person_Rec::m_id)
			),
			make_table("LINE_RESPONSABILITIES"s,
				make_column("statement_id",
					&LineResponsibility_Rec::m_statement_fid),
				make_column("responsible_id",
					&LineResponsibility_Rec::m_responsible_fid),
				foreign_key(&LineResponsibility_Rec::m_statement_fid).references(&StatementLine_Rec::m_id),
				foreign_key(&LineResponsibility_Rec::m_responsible_fid).references(&Responsible_Rec::m_id)
			),
			make_table("STATEMENTLINES"s,
				make_column("id",
					&StatementLine_Rec::m_id,
					primary_key()),
				make_column("account",
					&StatementLine_Rec::m_account_fid),
				make_column("line_date",
					&StatementLine_Rec::m_lineDate),
				make_column("concept",
					&StatementLine_Rec::m_concept_fid),
				make_column("colones"s,
					&StatementLine_Rec::m_amountInLocal),
				make_column("dolares"s,
					&StatementLine_Rec::m_amountInDollars),
				make_column("category"s,
					&StatementLine_Rec::m_category_fid),
				make_column("enabled",
					&StatementLine_Rec::m_enabled),
				make_column("details",
					&StatementLine_Rec::m_details),
				make_column("payment_to",
					&StatementLine_Rec::m_payment_to_fid),
				foreign_key(&StatementLine_Rec::m_account_fid).references(&Account_Rec::m_number_id),
				foreign_key(&StatementLine_Rec::m_category_fid).references(&Category_Rec::m_name_id),
				foreign_key(&StatementLine_Rec::m_concept_fid).references(&Concept_Rec::m_concept_id),
				foreign_key(&StatementLine_Rec::m_payment_to_fid).references(&Account_Rec::m_number_id)
				));

#if 0
	foreign_key(&StatementLine_Rec::m_category_fid).references(&Category_Rec::m_name_id),
	foreign_key(&StatementLine_Rec::m_account_fid).references(&Account_Rec::m_number_id)));
#endif
		if (flag == 0)
		{
			flag = 1;
			storage.sync_schema();
		}
		return storage;
	}
	void fillDB();
}
