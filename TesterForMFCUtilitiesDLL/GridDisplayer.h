#pragma once

template<typename T, typename DataType1, typename DataType2> //typename ...Cols>
class GridDisplayer
{
	std::vector<T>	lines;
	CJDGridCtrl& grid;
	std::vector<std::string> headers;
public:
	GridDisplayer(CJDGridCtrl& grid, 
		std::vector<T>&& lines,
		std::vector<std::string>&& headers) 
	: lines{std::move(lines)}, grid{grid}, headers{ std::move(headers)}
	{
		grid.SetColumnCount( 2 /*sizeof...(Cols)*/ + 1);
		grid.SetRowCount(lines.size() + 1);
		int col = 1;
		for( auto& str : headers)
		{
			auto head = JD::to_cstring(str);
			grid.SetItemText(0, col, head);
			++col;
		}
		for (int row = 0; row < lines.size(); ++row)
		{
			Display(row+1, 1, Cols...);
		}
	}
	template<typename Head, typename ...Rest>
	void Display(int display_row, int display_col, Head head, Rest... rest)
	{
		CString asText = print(head);
		grid.SetItemText(display_row, display_col, asText);
		Display(display_row, ++display_col, rest...);
	}
	template<typename Head>
	void Display(int display_row, int display_col, Head head)
	{

	}
	template<typename Col>
	void print(T::Col col)
	{
		
	}
};
