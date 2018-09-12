#pragma once

#include <vector>
#include <string>
#include <cassert>

#include <FixedPoint/Money.h>
#include <date/date.h>
#ifndef DLLS_H
#define MFC_UTILITIES_EXPORTS
#include <Dlls.h>
#endif



struct MFC_UTILITIES_API CellPosition
{
private:
	std::size_t m_row;
	std::size_t m_col;
public:
	CellPosition( const std::initializer_list<size_t>& coordinates)
		: m_row{ *coordinates.begin()},
		m_col { *std::next( coordinates.begin())}
	{
		assert(coordinates.size() == 2);
	}
	std::size_t getRow() const noexcept { return m_row;  }
	std::size_t getCol() const noexcept { return m_col; }
};

// associated to one file but may be saved to different file names
class MFC_UTILITIES_API CSVFile
{
public:
	using Row = std::vector<std::string>;
	using StringMatrix = std::vector<Row>;
	using Iterator = StringMatrix::const_iterator;
	using ReverseIterator = StringMatrix::reverse_iterator;
	using ConstantIterator = StringMatrix::const_iterator;
	using ConstantReverseIterator = StringMatrix::const_reverse_iterator;
private:
	StringMatrix		m_matrix;
	// assumptions: all rows have the same number of columns
	Row m_currentRow;

	void addCol(const std::string& col);
	void addCol(date::sys_days pt);
	void addCol(Money money);
	void addEndl();

public:
	CSVFile();

	CSVFile(const CSVFile& other) = delete;
	~CSVFile() = default;

	// closes file after reading it!
	void load(const std::string& fileName);

	void saveAs(std::string outputFileName);

	// access functions (instead of using m_matrix directly):
	//std::string& operator()( const CellPosition& pos );
	ConstantReverseIterator crbegin() const { return m_matrix.crbegin(); }
	ConstantReverseIterator crend() const { return m_matrix.crend(); }

	ConstantIterator cbegin() const { return m_matrix.cbegin(); }
	ConstantIterator cend() const { return m_matrix.cend(); }

	std::string getString(const CellPosition& pos) const;
	date::sys_days getDate(const CellPosition& pos) const;
	Money getMoney(const CellPosition& pos) const;

	Row getRow(std::size_t rowNumber) const;

	// DONE: moved it to private!
	// StringMatrix		m_matrix;

	CSVFile& operator<<(const std::string& col);
	CSVFile& operator<<(date::sys_days pt);
	CSVFile& operator<<( Money money);



//	typedef std::basic_ostream<char>&  (__cdecl *Manip)(std::basic_ostream<char>&);
	using Manipulator = std::basic_ostream<char>&  (__cdecl *)(std::basic_ostream<char>&);

	CSVFile& operator<<(Manipulator func);


	// accessors
	std::string getText(int row, int col) const noexcept;
	size_t getRowCount() const noexcept;
	size_t getColCount() const noexcept;
};
