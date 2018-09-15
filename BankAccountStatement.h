#pragma once


#include <memory>
#include <chrono>
#include <date.h>
#include "Money.h"
#include "CSVFile.h"
#include "Utilities.h"
#include "Category.h"
#include "Statement.h"

class StatementLine;
class Category;
extern bool isBACAccountNumber(std::string text);

class BankAccountStatement : public Statement
{
private:
	using Range = std::pair<CSVFile::Iterator, CSVFile::Iterator>;

	//std::string												m_fileName;
	//CSVFile													m_inputFile;
	//std::string												m_filePath;
	//date::sys_days											m_statementDate;

	std::string					m_owner;
	std::string					m_account;
	bool						m_isDollarAccount;
	Money						m_initialBalance;
	Money						m_booksBalance;
	Money						m_retainedDiffered;
	Money						m_availableBalance;

	std::vector<std::shared_ptr<StatementLine>>	m_statementLines;

	//date::sys_days extractDate(const std::string& fileName);

	void loadHeaderStatement();
	void loadStatementLines();

	date::sys_days linedate(CSVFile::Iterator p) const noexcept;

	std::string concept(CSVFile::Iterator p) const noexcept;

	Money cashAmountInLocal(CSVFile::Iterator p) const noexcept;

	Money cashAmountInDollars(CSVFile::Iterator p) const noexcept;

	void addStatementLine(std::shared_ptr<StatementLine> line);
	void addStatementLine(date::sys_days dp, const std::string & concept, Money amountInColones, Money amountInDollars, const std::shared_ptr<Category>& cat = categoryList.getNullCategory());

	BankAccountStatement::Range findStatementLineRange(CSVFile::Iterator start, CSVFile::Iterator end);

	void saveCSVAs(const std::string & outputFileName) const;

	void saveArchivedAs(const std::string & outputFileName) const;

	void saveTo(CArchive & ar) const noexcept;

	std::string getCSVFileName() const noexcept;

	std::string getArchivedFileName() const noexcept;

	void doLoad() override;
	void doSaveAll() const override;

public:

	explicit BankAccountStatement(const std::string& fileName, date::sys_days dp);
	~BankAccountStatement();

	static std::shared_ptr<BankAccountStatement> loadFrom(CArchive & ar) noexcept;

	const std::vector<std::shared_ptr<StatementLine>>& getStatementLines() const noexcept { return m_statementLines; }

	enum class FileType
	{
		CommaSeparatedValues, ArchivedFile
	};

	// on file names and paths:
	//static std::string getFilePath(const std::string& fileName);

	static std::string constructFileName(const std::string& filePath, const std::string& accountNumber, date::sys_days statementDate, bool isParentStatement, FileType fileType) noexcept;

	static std::string constructFileName(const std::string & filePath, date::sys_days statementsDate, FileType fileType) noexcept;

	// on interpreting matrix contents
	std::string owner() const noexcept
	{
		return getString({ 1,1 });
	}

	std::string account() const noexcept
	{
		return getString({ 1,2 });
	}
	bool isDollars() const noexcept
	{
		auto text = getString({ 1,3 });
		return text == "USD";
	}
	Money initialBalance() const noexcept
	{
		return getMoney ({ 1,4 });
	}
	Money booksBalance() const noexcept
	{
		return getMoney ({ 1,5 });
	}
	Money retainedDiffered() const noexcept
	{
		return getMoney({ 1,6 });
	}
	Money availableBalance() const noexcept
	{
		return getMoney({ 1,7 });
	}

};

