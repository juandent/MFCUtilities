#pragma once

#include <chrono>
#include <date/date.h>
#include <FixedPoint/Money.h>
#include "CSVFile.h"

#include "Statement.h"
#include "StatementBuilder.h"

class ParentCreditCardStatement;
class ChildCreditCardStatement;
extern bool isBACAccountNumber(std::string text);

class CreditCardStatement : public DataTier::Statement
{
private:
	using Range = std::pair<CSVFile::Iterator, CSVFile::Iterator>;

	//std::string												m_fileName;
	//CSVFile													m_inputFile;
	//std::string												m_filePath;
	std::shared_ptr<ParentCreditCardStatement>				m_parentStatement;
	std::vector<std::shared_ptr<ChildCreditCardStatement>>	m_childStatements;

	void loadIntoParentStatement();
	Range findParentStatementLineRange();
	Range findChildStatementLineRange(CSVFile::Iterator start, CSVFile::Iterator end);
	void loadParentStatementLines();
	void searchChildStatements();
	date::sys_days linedate(CSVFile::Iterator p) const noexcept;
	std::string concept(CSVFile::Iterator p) const noexcept
	{
		auto txt = (*p)[1];
		JD::diag::display(txt);
		return txt;
	}
	Money cashAmountInLocal(CSVFile::Iterator p) const noexcept
	{
		auto text = (*p)[2];
		return JD::to_money(text) * -1;
	}
	Money cashAmountInDollars(  CSVFile::Iterator p ) const noexcept
	{
		auto text = (*p)[3];
		return JD::to_money(text) * -1;
	}

	void doLoad() override;
	void doSaveAll() const override;

public:

	explicit CreditCardStatement( const std::string& fileName, date::sys_days dp);
	~CreditCardStatement();

	enum class FileType
	{
		CommaSeparatedValues, ArchivedFile
	};

	static std::string constructFileName(const std::string& filePath, const std::string& accountNumber, date::sys_days statementDate, bool isParentStatement, FileType fileType) noexcept;

	static std::string constructFileName(const std::string & filePath, date::sys_days statementsDate, CreditCardStatement::FileType fileType) noexcept;

	ParentCreditCardStatement*	getParentStatement() const noexcept
	{
		return m_parentStatement.get();
	}
	const std::vector<std::shared_ptr<ChildCreditCardStatement>>& getChildStatements() const noexcept;

	size_t getChildStatementCount() const noexcept
	{
		return m_childStatements.size();
	}
	std::string getChildStatementArchivedName(int i) const;

	std::string account() const noexcept
	{
		return getString({ 1,0 });
	}
	std::string owner() const noexcept
	{
		return getString({ 1,1 });
	}
	date::sys_days date() const
	{
		return getDate ({ 1,2 });
	}
	date::sys_days minPaymentDate() const
	{
		return getDate({ 1,3 });
	}
	Money minPaymentAmountInLocal() const noexcept
	{
		return getMoney ({ 1,4 });
	}
	Money minPaymentAmountInDollars() const noexcept
	{
		return getMoney({ 1,5 });
	}
	date::sys_days cashPaymentDate() const
	{
		return getDate({ 1,6 });
	}
	Money cashPaymentAmountInLocal() const noexcept
	{
		return getMoney ({ 1,7 });
	}
	Money cashPaymentAmountInDollars() const noexcept
	{
		return getMoney ({ 1,8 });
	}
	Money prevBalanceInLocal() const noexcept
	{
		return getMoney ({ 3,2 });
	}
	Money prevBalanceInDollars() const noexcept
	{
		return getMoney({ 3,3 });
	}
	std::string conceptForInterestAmountInLocal() const noexcept
	{
		return getString ({ getInputFile().getRowCount() - 2, 0 });
	}
	std::string conceptForInterestAmountInDollars() const noexcept
	{
		return getString({ getInputFile().getRowCount() - 2, 1 });
	}
	Money interestAmountInLocal() const noexcept
	{
		return getMoney({ getInputFile().getRowCount() - 1, 0 }) * -1;
	}
	Money interestAmountInDollars() const noexcept
	{
		return getMoney({ getInputFile().getRowCount() - 1, 1 }) * -1;
	}

};

