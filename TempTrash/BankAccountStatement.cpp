#include "stdafx.h"
#include "BankAccountStatement.h"
#include "StatementLines.h"
#include "CArchiveOperators.h"
#include "StatementLines.h"
#include "StatementCentral.h"
#include "AccountOwnerTable.h"

using namespace std;
namespace fs = std::experimental::filesystem;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Constructor.
/// 			Only StatementFactory can call this constructor! </summary>
///
/// <remarks>	Juan, 6/4/2017. </remarks>
///
/// <param name="fileName">	Filename of the file. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

BankAccountStatement::BankAccountStatement(const std::string & fileName, date::sys_days dp)
	: Statement(fileName, dp)
{
}

BankAccountStatement::~BankAccountStatement()
{
}

#if 0
// find date inside the filename -- must be complete: 2016-10-03
date::sys_days BankAccountStatement::extractDate(const std::string& fileName)
{
	auto file = JD::remove_extension(fileName);
	// remove chars before date
	smatch m;
	regex reg{ "[^[:digit:]]*([[:digit:]]{4}-[[:digit:]]{1,2}-[[:digit:]]{1,2})" };

	bool ok = regex_search( file, m, reg );

	if (!ok)
	{
		throw ios_base::failure("Name should have format like 2017-01-12");
	}
	//assert(ok);

	string dateInFilename = m.str(1);

	return JD::to_date(dateInFilename);
}
#endif

void BankAccountStatement::loadHeaderStatement()
{
	this->m_account = this->account();
	this->m_availableBalance = this->availableBalance();
	this->m_booksBalance = this->booksBalance();
	this->m_initialBalance = this->initialBalance();
	this->m_isDollarAccount = this->isDollars();
	this->m_owner = this->owner();
	this->m_retainedDiffered = this->retainedDiffered();
}

void BankAccountStatement::loadStatementLines()
{
	auto range = findStatementLineRange( getInputFile().cbegin(), getInputFile().cend());
	if (range.first == range.second)
	{
		TRACE(L"No lines in Bank Account Statement!\n");
		return;		// NO LINES!
	}

	for (auto pos = range.first; pos != range.second; ++pos)
	{
		auto ld = linedate(pos);
		auto con = concept(pos);
		auto local = cashAmountInLocal(pos);
		auto dollars = cashAmountInDollars(pos);

		addStatementLine(ld, con, local, dollars);
	}
}

date::sys_days BankAccountStatement::linedate(CSVFile::Iterator p) const noexcept
{

	auto text = (*p)[0];
	return JD::to_date(text); 
}

std::string BankAccountStatement::concept(CSVFile::Iterator p) const noexcept
{

	auto text = (*p)[3];
	return text;
}

Money BankAccountStatement::cashAmountInLocal(CSVFile::Iterator p) const noexcept
{
	if (m_isDollarAccount)
	{
		return Money{ 0 };
	}
	else
	{
		auto debitText = (*p)[4];
		auto creditText = (*p)[5];

		auto debit = JD::to_money(debitText) * -1;	// debit is always negative
		auto credit = JD::to_money(creditText);
		return credit + debit;
	}
}

Money BankAccountStatement::cashAmountInDollars(CSVFile::Iterator p) const noexcept
{
	if (! m_isDollarAccount)
	{
		return Money{ 0 };
	}
	else
	{
		auto debitText = (*p)[4];
		auto creditText = (*p)[5];

		auto debit = JD::to_money(debitText) * -1;
		auto credit = JD::to_money(creditText);
		return debit + credit;
	}
}


void BankAccountStatement::addStatementLine(std::shared_ptr<StatementLine> line)
{
	m_statementLines.push_back(line);
}

void BankAccountStatement::addStatementLine(date::sys_days dp, const std::string& concept, Money amountInColones, Money amountInDollars, const std::shared_ptr<Category>& cat)
{
//	StatementLine l{ m_account, dp, concept, amountInColones, amountInDollars, cat };
	string owner = AccountOwnerTable::getOwner(m_account);

	auto  line = make_shared<StatementLine>( m_account, dp, concept, amountInColones, amountInDollars, owner, cat );
	m_statementLines.push_back(line);
}

BankAccountStatement::Range BankAccountStatement::findStatementLineRange(CSVFile::Iterator start, CSVFile::Iterator end)
{
	auto first = find_if(start, end, [](const CSVFile::Row& cols)
	{
		//cout << cols[0] /*<< " " << cols[1]*/ << endl;
		return JD::is_date(cols[0]);
	});

	if (first == end)
	{
		return make_pair(end, end);
	}

	auto last = find_if(first + 1, end, [](const CSVFile::Row& cols)
	{
		return cols[0].empty() || ! JD::is_date(cols[0]);
	});

	return make_pair(first, last);
}

void BankAccountStatement::doLoad()
{
	loadHeaderStatement();
	loadStatementLines();
}

void BankAccountStatement::doSaveAll() const
{
	auto csvFileName = getCSVFileName();
	saveCSVAs(csvFileName);
	auto archivedFileName = getArchivedFileName();
	saveArchivedAs(archivedFileName);
}

std::string BankAccountStatement::getCSVFileName() const noexcept
{
	return StatementCentral::constructFileName( getFilePath(), getStatementDate(), StatementCentral::FileType::CommaSeparatedValues, StatementCentral::StatementType::BankAccount, m_account);
}

std::string BankAccountStatement::getArchivedFileName() const noexcept
{
	return StatementCentral::constructFileName( getFilePath(), getStatementDate(), StatementCentral::FileType::BankAccountStatement, StatementCentral::StatementType::BankAccount, m_account);
}

void BankAccountStatement::saveCSVAs(const std::string & outputFileName) const
{
	CSVFile out{};

	out << m_account;
	out << m_owner;
	out << getStatementDate();
	out << m_isDollarAccount;
	out << m_initialBalance;
	out << m_booksBalance;
	out << m_retainedDiffered;
	out << m_availableBalance;
	out << endl;

	for (auto& row : m_statementLines)
	{
		row->saveToCSV(out);
	}

	out.saveAs(outputFileName);
}

void BankAccountStatement::saveArchivedAs(const std::string & outputFileName) const
{
	CFile file{ JD::to_cstring(outputFileName), CFile::modeCreate | CFile::modeWrite };
	CArchive ar{ &file, CArchive::Mode::store };

	saveTo(ar);

}

void BankAccountStatement::saveTo(CArchive & ar) const noexcept
{
	ar << m_account;
	ar << m_owner;
	ar << getStatementDate();
	ar << m_isDollarAccount;
	ar << m_initialBalance;
	ar << m_booksBalance;
	ar << m_retainedDiffered;
	ar << m_availableBalance;

	ar << m_statementLines.size();

	for (auto& row : m_statementLines)
	{
		row->saveToCArchive(ar);
	}
}

std::shared_ptr<BankAccountStatement> BankAccountStatement::loadFrom(CArchive & ar) noexcept
{
	auto fileNameCs = ar.GetFile()->GetFilePath();
	auto fileName = JD::from_cstring(fileNameCs);

	auto filePath = Statement::getFilePath(fileName);

	string account, owner;
	date::sys_days statementDate;
	ar >> account;
	ar >> owner;
	ar >> statementDate;

	auto statement = make_shared<BankAccountStatement>(fileName, statementDate);
	statement->m_account = account;
	statement->m_owner = owner;
	//statement->m_statementDate = statementDate;
	ar >> statement->m_isDollarAccount;
	ar >> statement->m_initialBalance;
	ar >> statement->m_booksBalance;
	ar >> statement->m_retainedDiffered;
	ar >> statement->m_availableBalance;

	date::sys_days lineDate;
	//string lineConcept;
	Money amountInLocal, amountInDollars;

	size_t lineCount;

	ar >> lineCount;

	for (auto i = 0u; i < lineCount; ++i)
	{
		auto line = StatementLine::loadFrom(ar);
		statement->addStatementLine(line);
	}

	return statement;
}
