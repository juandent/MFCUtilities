#include "stdafx.h"
#include "CreditCardStatement.h"
#include "ParentCreditCardStatement.h"
#include "ChildCreditCardStatement.h"
#include <filesystem>

using namespace std;
namespace fs = std::experimental::filesystem;


CreditCardStatement::CreditCardStatement( const string& fileName, date::sys_days dp)
	: Statement{fileName, dp},
	m_parentStatement {	make_shared<ParentCreditCardStatement>(Statement::getFilePath())}
{
}


CreditCardStatement::~CreditCardStatement()
{
}


class firstNonEmptyDate
{
public:
	bool operator()(const CSVFile::Row& cols)
	{
		//std::cout << cols[1] << std::flush;
		return ! cols[0].empty();
	}
};

const std::vector<std::shared_ptr<ChildCreditCardStatement>>&  CreditCardStatement::getChildStatements() const noexcept
{
	return m_childStatements;
}

std::string CreditCardStatement::getChildStatementArchivedName(int i) const
{
	return m_childStatements[i]->getArchivedFileName();
}


void CreditCardStatement::loadIntoParentStatement()
{
	m_parentStatement->setAccount(account());
	m_parentStatement->setCashPaymentDate(cashPaymentDate());
	m_parentStatement->setCashPaymentDollars(cashPaymentAmountInDollars());
	m_parentStatement->setCashPaymentLocal(cashPaymentAmountInLocal());
	m_parentStatement->setMinPaymentDate(minPaymentDate());
	m_parentStatement->setMinPaymentDollars(minPaymentAmountInDollars());
	m_parentStatement->setMinPaymentLocal(minPaymentAmountInLocal());
	m_parentStatement->setOwner(owner());
	m_parentStatement->setPrevBalanceDollars(prevBalanceInDollars());
	m_parentStatement->setPrevBalanceLocal(prevBalanceInLocal());
	m_parentStatement->setStatementDate(this->date());

	loadParentStatementLines();
}

CreditCardStatement::Range CreditCardStatement::findParentStatementLineRange()
{
	// find backwards for first non empty concept (but start 2 rows before the end of the m_matrix)
	auto rpos = find_if( getInputFile().crbegin() + 2, getInputFile().crend(), firstNonEmptyDate());

	CSVFile::Iterator firstLineToAdd = rpos.base();

	auto first_statement_line_for_parent = distance( getInputFile().cbegin(), firstLineToAdd);

	cout << "first_statement_line_for_parent is: " << first_statement_line_for_parent << endl;

	// find range
	auto lastLineToAdd = find_if_not(firstLineToAdd, getInputFile().cend() - 2, [](const CSVFile::Row& cols)
	{
		cout << cols[0] << " " << cols[1] << endl;
		return cols[0].empty();
	});

	return make_pair(firstLineToAdd, lastLineToAdd);
}

CreditCardStatement::Range CreditCardStatement::findChildStatementLineRange(CSVFile::Iterator start, CSVFile::Iterator end)
{
	auto first = find_if(start, end, [](const CSVFile::Row& cols)
	{
		cout << cols[0] << " " << cols[1] << endl;
		return cols[0].empty() &&  isBACAccountNumber(cols[1]);
	});

	if (first == end)
	{
		return make_pair(end, end);
	}

	auto last = find_if(first + 1, end, [](const CSVFile::Row& cols)
	{
		return cols[0].empty();
	});

	return make_pair(first, last);
}

void CreditCardStatement::loadParentStatementLines()
{
	auto range = findParentStatementLineRange();

	for ( auto pos = range.first; pos != range.second; ++pos)
	{
		//cout << (*pos)[0] << (*pos)[1] << endl;
		m_parentStatement->addStatementLine(concept(pos), cashAmountInLocal(pos), cashAmountInDollars(pos));
	}

	/// Add statementline while first column is empty
	// end when  first column is not empty

	m_parentStatement->addStatementLine(conceptForInterestAmountInLocal(), interestAmountInLocal(), 0);
	m_parentStatement->addStatementLine(conceptForInterestAmountInDollars(), 0, interestAmountInDollars());
}

// TODO: consider renaming to searchAndLoadChildStatements()
void CreditCardStatement::searchChildStatements()
{
	auto range = findChildStatementLineRange( getInputFile().cbegin(), getInputFile().cend());

	while (range.first != range.second)
	{
		std::string account = (*range.first)[1];

		std::shared_ptr<ChildCreditCardStatement> child{ new ChildCreditCardStatement{ Statement::getFilePath(), account, m_parentStatement->getOwner(), m_parentStatement->getStatementDate() } };

		for (auto pos = range.first + 1; pos != range.second; ++pos)
		{
			auto ld = linedate(pos);
			auto con = concept(pos);
			auto local = cashAmountInLocal(pos);
			auto dollars = cashAmountInDollars(pos);

			child->addStatementLine(ld, con, local, dollars);
		}

		m_childStatements.push_back(child);

		range = findChildStatementLineRange(range.second, getInputFile().cend());
	}
}

date::sys_days CreditCardStatement::linedate(CSVFile::Iterator p) const noexcept
{
	
	auto text = (*p)[0];
	date::sys_days statementDate = m_parentStatement->getStatementDate();
	date::year_month_day ymd{ statementDate };

	return JD::to_date(text, static_cast<unsigned>(ymd.month()), static_cast<int>(ymd.year()));

	//return std::chrono::time_point<std::chrono::system_clock, date::days>{};
}

void CreditCardStatement::doLoad()
{
	loadIntoParentStatement();
	searchChildStatements();
}

void CreditCardStatement::doSaveAll() const 
{
	// save parent statement
	m_parentStatement->save();
	// save each child statement
	for (auto& child : m_childStatements)
	{
		child->save();
	}
}

bool isBACAccountNumber(std::string text)
{
	//  5491-94**-****-7551

	//regex accountPattern( "[[:d:]]{4}\-[[:d:]]{2}[*]{2}\-[*]{4}\-[[:d:]]{4}");
	regex accountPattern("[[:d:]]{4}-[[:d:]]{2}[*]{2}-[*]{4}-[[:d:]]{4}");


	bool isAccount = regex_match(text, accountPattern);

	return isAccount;
}
