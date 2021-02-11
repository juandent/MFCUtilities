#pragma once

import Util;

template<typename T, typename ColonesCols, typename DollarsCols, int NumCols>
class GridDisplayer
{
	CJDGridCtrl&				grid;
	using Container = std::vector<std::remove_reference_t<T>>;
	Container lines;
	using ValueType = typename Container::value_type;
	std::vector<std::string>	headers;
	// const size_t NumCols = headers.size();
public:
	GridDisplayer(CJDGridCtrl& grid, 
		Container&& lines_,
		std::vector<std::string>&& headers_) 
	: lines{std::move(lines_)}, grid{grid}, headers{ std::move(headers_)}
	{
		grid.SetColumnCount( headers.size() + 1);
		grid.SetRowCount(lines.size() + 1);
		grid.SetFixedRowCount();
		grid.SetFixedColumnCount();
		grid.SetHeaderSort(true);
		grid.SetSingleRowSelection(true);

		grid.m_sortingFunctions.resize(NumCols + 1);
		grid.m_sortingFunctions[0] = Util::Comparison::Text;

		int col = 1;
		for( auto& str : headers)
		{
			auto head = Util::to_cstring(str);
			grid.SetItemText(0, col, head);
			++col;
		}
	}

	template<typename ...DataTypes>
	void display(DataTypes... dts)
	{
		// constexpr size_t NumCols = sizeof(dts);
		static_assert(sizeof...(dts) == NumCols, "Error in number of columns in GridDisplayer");
		
		RECT rect;
		grid.GetWindowRect(&rect);

		for (int i = 0; i < lines.size(); ++i)
		{
			// printDataInGrid(i, lines[i], dts...);
			PrintDataInGrid<0, Container, NumCols>::template Apply(i, lines, grid, dts...);
		}
		grid.SetColumnWidth(0, 100);
		int width = grid.GetColumnWidth(0);
		for (int i = 0; i < headers.size(); ++i)
		{
			grid.AutoSizeColumn(i+1);	// skip vertical headers
			width += grid.GetColumnWidth(i + 1);
		}
		auto frame = grid.GetParentFrame();
		width += GetSystemMetrics(SM_CXVSCROLL);
		grid.SetWindowPos(frame, 10, 10, width, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);
	}

private:
	template<int Col, typename Container, unsigned NumCols>
	struct PrintDataInGrid
	{
		template<typename DataType>
		static void Apply(int row, const Container& z, CJDGridCtrl& grid, DataType p)
		{
			auto value = z[row].*p;
			using ValueType = std::remove_reference_t<decltype(value)>;

			CString cs;

			if constexpr (std::is_integral_v<ValueType> || std::is_floating_point_v<ValueType>)
			{
				grid.m_sortingFunctions[Col] = Util::Comparison::Money;

				if (ColonesCols::template found<Col >())
				{
					Util::Colones c(value);
					cs = format(c);
				}
				else if (DollarsCols::template found<Col>())
				{
					Util::Dolares d(value);
					cs = format(d);
				}
				else
				{
					cs = format(value);
				}
			}
			else
			{
				grid.m_sortingFunctions[Col] = Util::Comparison::Text;
				cs = format(value);
			}
			grid.SetItemText(row + 1, Col, cs);
		}
		template<typename DataType, typename ... RestDataTypes>
		static void Apply(int row, const Container& z, CJDGridCtrl& grid, DataType p, RestDataTypes...dts)
		{
#if 0			
			auto value = z[row].*p;
			//auto value = z.*p;
			
			using ValueType = int;// decltype(value);

			CString cs;

			if constexpr (std::is_integral_v<ValueType> ) // || std::is_floating_point_v<ValueType>)
			{
				value++;
#if 0				
				grid.m_sortingFunctions[Col + 1] = Util::Comparison::Money;

				if (ColonesCols::template found<Col + 1>)
				{
					Util::Colones c(value);
					cs = format(c);
				}
				else if (DollarsCols::template found<Col + 1>())
				{
					Util::Dolares d(value);
					cs = format(d);
				}
				else
				{
					cs = format(value);
				}
#endif
			}
			else
			{
				grid.m_sortingFunctions[Col + 1] = Util::Comparison::Text;
				cs = format(value);
			}

			//auto cs = format(value);
			grid.SetItemText(row + 1, Col, cs);

#endif
			PrintDataInGrid<Col, Container, NumCols>::Apply(row, z, grid, p);
			PrintDataInGrid<Col + 1, Container, NumCols>::Apply(row, z, grid, dts...);
		}
	};

	template<typename Container>
	struct PrintDataInGrid<NumCols, Container, NumCols>
	{
		template<typename ...DataTypes>
		static void Apply(int row, const Container& z, CJDGridCtrl& grid, DataTypes...dts)
		{
		}
	};
#if 0	
	template<int Col, typename DataType>
	void printDataInGrid(int row, T& z, DataType p)
	{
		auto value = z.*p;
		using ValueType = decltype(value);

		CString cs;

		if constexpr (std::is_integral_v<ValueType> || std::is_floating_point_v<ValueType>)
		{
			grid.m_sortingFunctions[Col + 1] = Util::Comparison::Money;

			if( ColonesCols::template found<Col+1>)
			{
				Util::Colones c(value);
				cs = format(c);
			}
			else if (DollarsCols::template found<Col+1>())
			{
				Util::Dolares d(value);
				cs = format(d);
			}
			else
			{
				cs = format(value);
			}
		}
		else
		{
			grid.m_sortingFunctions[Col + 1] = Util::Comparison::Text;
			cs = format(value);
		}
		
		//auto cs = format(value);
		grid.SetItemText(row + 1, Col, cs);
	}


	template<int Col, typename DataType, typename ... RestDataTypes>
	void printDataInGrid(int row, T& z, DataType p, RestDataTypes...qs)
	{
#if 0
		auto value = z.*p;
		auto cs = format(value);
		grid.SetItemText(row + 1, col, cs);
#endif
		printDataInGrid<Col>(row, z, p);
		printDataInGrid<Col+1>(row, z, qs...);
	}
#endif
	template<typename T>
	static CString format(const T& t)
	{
		return Util::to_cstring(t);
	}
	template<typename T>
	static CString format(const std::shared_ptr<T>& t)
	{
		if (t)
		{
			return Util::to_cstring(*t);
		}
		return L"";
	}
};
