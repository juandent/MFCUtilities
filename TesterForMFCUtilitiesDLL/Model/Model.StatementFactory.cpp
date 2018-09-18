#include "stdafx.h"
#include "StatementFactory.h"
#include "CSVFile.h"

// subclasses of Statement;
//#include "creditcardstatement.h"
//#include "BankAccountStatement.h"

using namespace std;
using namespace DataTier;

AccountType StatementFactory::selectType(const CSVFile & csv)
{
	auto cols = csv.getRow(0);

	if (cols[3] == "Moneda")
		return AccountType::BankAccount;
	if(cols[0].substr(0, 3) == "Pro")
		return AccountType::CreditCard;
	
	return AccountType::Unknown;
}

std::shared_ptr<Statement> StatementFactory::Create(const std::string & rawFileName, date::sys_days chosenDate)
{
	CSVFile file{};
	file.load(rawFileName);

	switch (selectType(file))
	{
	case AccountType::BankAccount:
		return nullptr; // make_shared<BankAccountStatement>(rawFileName, chosenDate);
	case AccountType::CreditCard:
		return nullptr; // make_shared<CreditCardStatement>(rawFileName, chosenDate);
	default:
		throw runtime_error{ "Unknown statement type in factory!" };
	}
}
