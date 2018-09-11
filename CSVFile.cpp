#include "stdafx.h"
#include "CSVFile.h"

using namespace std;


CSVFile::CSVFile()
{
}

// load from file into matrix
void CSVFile::load(const std::string & fileName)
{
	ifstream in{ fileName, ios_base::_Nocreate };

	if (!in)
	{
		ostringstream os;
		os << "Cannot open file '" << fileName << "'";
		throw ios_base::failure{ os.str() };
	}

	char buffer[100'000];

	int rowNumber = 0;
	m_matrix.resize(0);
	in.seekg(ios::beg);
	in.getline(buffer, sizeof buffer);

	while ( in.good())
	{
		vector<string> columns;
		string strBuffer{ buffer };
		JD::diag::display(strBuffer);

#if 0
		regex double_quote{ "\"" };

		string quotes_replaced;
		auto strInserter = back_inserter(quotes_replaced);
		regex_replace( strInserter, 
			strBuffer.begin(), strBuffer.end(), double_quote, ".");
		JD::diag::display(quotes_replaced);
#endif

		regex sep("[ \t\n]*[,][ \t\n]*");  // separated by , and spaces

		sregex_token_iterator p(strBuffer.cbegin(), strBuffer.cend(),  // sequence
			sep,														// separator
			-1);														// -1: values between separators

		sregex_token_iterator e;
		for (; p != e; ++p) {
			auto str = p->str();
			JD::diag::display(str);
			if (str[0] == '"')
			{
				//str = str.substr(1);	// skip the double quote '"'
				do {
					++p;
					str += "," + p->str();
					JD::diag::display(str);
					//auto pos_of_double_quote = str.find("\"");
					auto lastCharIter = str.rbegin();
					if ( *lastCharIter == '"')	// if last char is the closing double quote
					{
						//*lastCharIter = '\0';
						JD::diag::display(str);
						break;
					}
				} while (true);
			}
			columns.push_back(str);
		}

		m_matrix.push_back(columns);
		// read next line
		in.getline(buffer, sizeof buffer);
		rowNumber++;
	}
}

template<typename T>
std::string ProcessCodeException(const T & e)
{
	using namespace std;

	auto c = e.code();

	ostringstream os;

	os << c.category().name() << " " << c.value() << " " << c.message() << " ";

	return os.str();
}

void CSVFile::saveAs(std::string outputFileName)
{
	ofstream out;
	out.exceptions(ios::failbit | ios::badbit);

	try
	{
		out.open( /*"5555-****"*/ outputFileName, ios_base::trunc );
	}
	catch( ios_base::failure& exc)
	{
		string msg = ProcessCodeException(exc);

		auto err = GetLastError();
		char buff[1000];
		strerror_s(buff, err);

		ostringstream os;
		os << "Cannot open file '" << outputFileName << "' " << buff;
		throw ios_base::failure{ os.str() };
	}

	for ( auto& r = m_matrix.begin(); r != m_matrix.end(); ++r)
	{
		for (auto& c = r->begin(); c != r->end(); ++c)
		{
			out << *c;
			if (next(c) != r->end())
			{
				out << ",";
			}
		}
		out << endl;
	}
}

#if 0
std::string & CSVFile::operator()(const CellPosition & pos)
{
	return m_matrix[pos.getRow()][pos.getCol()];
}
#endif

std::string CSVFile::getString(const CellPosition & pos) const
{
	return m_matrix[pos.getRow()][pos.getCol()];
}

date::sys_days CSVFile::getDate(const CellPosition & pos) const
{
	auto text = m_matrix[pos.getRow()][pos.getCol()];
	return JD::to_date(text);
}

Money CSVFile::getMoney(const CellPosition & pos) const
{
	auto text = m_matrix[pos.getRow()][pos.getCol()];
	return JD::to_money(text);		// makes negative values deposits
}

CSVFile::Row CSVFile::getRow(std::size_t rowNumber) const
{
	return m_matrix[rowNumber];
}

CSVFile & CSVFile::operator<<(const std::string & col)
{
	addCol(col);
	return *this;
}

CSVFile & CSVFile::operator<<(date::sys_days pt)
{
	addCol(pt);
	return *this;
}

CSVFile & CSVFile::operator<<(Money money)
{
	addCol(money);
	return *this;
}

CSVFile & CSVFile::operator<<(Manipulator func)
{
	Manipulator m = std::endl;
	if (func == m)
	{
		addEndl();
		return *this;
	}
	assert(!"Another manipulator passed");
	return *this;
}

void CSVFile::addCol(const std::string & col)
{
	m_currentRow.push_back(col);
}

void CSVFile::addCol(date::sys_days pt)
{
#define USING_PLUS_FOR_DATE
#ifdef USING_PLUS_FOR_DATE
	date::year_month_day ymd{ pt };
	ostringstream os;
	// to enable correct importing into FileMaker Pro
	os << ymd.year();
	os << "+";
	os << static_cast<unsigned>(ymd.month());
	os << "+";
	os << ymd.day();
	//os << ymd;
	addCol(os.str());
#else	// dd/mm/yyyy
	date::year_month_day ymd{ pt };
	ostringstream os;

	os.fill('0');
	os.width(2);
	os.flags(std::ios::dec | std::ios::right);
	os << ymd.day() << "/";
	os.width(2);
	os << static_cast<unsigned>(ymd.month()) << '/';
	os.width(4);
	os << ymd.year();
	addCol(os.str());
#endif
}

void CSVFile::addCol(Money money)
{
	ostringstream os;
	os << money;
	addCol(os.str());
}

void CSVFile::addEndl()
{
	m_matrix.push_back(m_currentRow);
	m_currentRow.clear();
}

std::string CSVFile::getText(int row, int col) const noexcept
{
	return m_matrix[row][col];
}

size_t CSVFile::getRowCount() const noexcept
{
	return m_matrix.size();
}
// assume all rows have same amount of columns!
size_t CSVFile::getColCount() const noexcept
{
	return m_matrix[0].size();
}
