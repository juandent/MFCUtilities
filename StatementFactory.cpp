#include "stdafx.h"
#include "StatementFactory.h"
// subclasses of Statement;
#include "creditcardstatement.h"
#include "BankAccountStatement.h"

using namespace std;

StatementTypes StatementFactory::selectType(const CSVFile & csv)
{
	auto cols = csv.getRow(0);

	if (cols[3] == "Moneda")
		return StatementTypes::BAC_BankAccount;
	if(cols[0].substr(0, 3) == "Pro")
		return StatementTypes::BAC_CreditCard;
	
	return StatementTypes::Unkwnon;
}

std::shared_ptr<Statement> StatementFactory::Create(const std::string & rawFileName, date::sys_days chosenDate)
{
	CSVFile file{};
	file.load(rawFileName);

	switch (selectType(file))
	{
	case StatementTypes::BAC_BankAccount:
		return make_shared<BankAccountStatement>(rawFileName, chosenDate);
	case StatementTypes::BAC_CreditCard:
		return make_shared<CreditCardStatement>(rawFileName, chosenDate);
	default:
		throw runtime_error{ "Unknown statement type in factory!" };
	}
}
