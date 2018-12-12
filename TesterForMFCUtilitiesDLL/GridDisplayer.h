#pragma once



template<typename T> 
class GridDisplayer
{
	CJDGridCtrl&				grid;
	std::vector<T>				lines;
	std::vector<std::string>	headers;
public:
	GridDisplayer(CJDGridCtrl& grid, 
		std::vector<T>&& lines_,
		std::vector<std::string>&& headers_) 
	: lines{std::move(lines_)}, grid{grid}, headers{ std::move(headers_)}
	{
		grid.SetColumnCount( headers.size() + 1);
		grid.SetRowCount(lines.size() + 1);
		grid.SetFixedRowCount();
		grid.SetFixedColumnCount();
		grid.SetHeaderSort(true);
		grid.SetSingleRowSelection(true);

		int col = 1;
		for( auto& str : headers)
		{
			auto head = JD::to_cstring(str);
			grid.SetItemText(0, col, head);
			++col;
		}
	}

	template<typename ...DataTypes>
	void display(DataTypes... dts)
	{
		for (int i = 0; i < lines.size(); ++i)
		{
			printDataInGrid(i, 1, lines[i], dts...);
		}
		for (int i = 0; i < headers.size(); ++i)
		{
			grid.AutoSizeColumn(i+1);	// skip vertical headers
		}
	}

private:
	template<typename DataType>
	void printDataInGrid(int row, int col, T& z, DataType p)
	{
		auto value = z.*p;
		auto cs = format(value);
		grid.SetItemText(row + 1, col, cs);
	}


	template<typename DataType, typename ... RestDataTypes>
	void printDataInGrid(int row, int col, T& z, DataType p, RestDataTypes...qs)
	{
		auto value = z.*p;
		auto cs = format(value);
		grid.SetItemText(row + 1, col, cs);
		printDataInGrid(row, ++col, z, qs...);
	}
	template<typename T>
	CString format(const T& t)
	{
		return JD::to_cstring(t);
	}
	template<typename T>
	static CString format(const std::shared_ptr<T>& t)
	{
		if (t)
		{
			return JD::to_cstring(*t);
		}
		return L"";
	}
};
