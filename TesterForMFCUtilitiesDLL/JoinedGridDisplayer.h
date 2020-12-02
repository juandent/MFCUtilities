#pragma once
#include <optional>

#include "..\ExpenseManagerSQLite/IDisplayer.h"

#if 0 // DEPRECATED
template<int N>
struct ColonesFormat
{
	std::array<unsigned, N> colsToFormat;
	ColonesFormat(const std::initializer_list<unsigned>& list)
	{
		std::copy(list.begin(), list.end(), colsToFormat.begin());
	}
	bool IsColonFormat(const unsigned col) const
	{
		return (std::find(colsToFormat.begin(), colsToFormat.end(), col) != colsToFormat.end());
	}
};

template<int N>
struct DolaresFormat
{
	std::array<unsigned, N> dolsToFormat;
	DolaresFormat(const std::initializer_list<unsigned>& list)
	{
		std::copy(list.begin(), list.end(), dolsToFormat.begin());
	}
	bool IsDollarFormat(const unsigned col) const
	{
		return (std::find(dolsToFormat.begin(), dolsToFormat.end(), col) != dolsToFormat.end());
	}
};
#endif

// template<typename T> colonesList;

template<typename T, typename ColonesCols, typename DollarsCols>
class JoinedGridDisplayer  :  public IDisplayer
{
	using Container = std::vector<std::remove_reference_t<T>>;
	using RowType = typename Container::value_type;
	inline static constexpr size_t NumCols = std::tuple_size<RowType>::value;
	CJDGridCtrl&				grid;
	Container					lines;
	std::vector<std::string>	headers;
public:
	JoinedGridDisplayer(CJDGridCtrl& grid,
		Container&& lines_,
		std::vector<std::string> headers_)
		: lines{ std::move(lines_) }, grid{ grid }, headers{ std::move(headers_) } 
	{
		grid.SetColumnCount(NumCols + 1); // headers.size() + 1);
		grid.SetRowCount(lines.size() + 1);
		grid.SetFixedRowCount();
		grid.SetFixedColumnCount();
		grid.SetHeaderSort(true);
		grid.SetSingleRowSelection(true);

		grid.m_sortingFunctions.resize(NumCols + 1);
		grid.m_sortingFunctions[0] = Util::Comparison::Text;

		int col = 1;
		for (auto& str : headers)
		{
			auto head = Util::to_cstring(str);
			grid.SetItemText(0, col, head);
			++col;
		}
		//static_assert(NumCols == MaxCol, "");
	}

	void display() override
	{
		RECT rect;
		grid.GetWindowRect(&rect);

		for (int i = 0; i < lines.size(); ++i)
		{
			PrintDataInGrid<0, Container, NumCols>::Apply(i, lines, grid);
		}
		grid.SetColumnWidth(0, 100);
		int width = grid.GetColumnWidth(0);
		for (int i = 0; i < headers.size(); ++i)
		{
			grid.AutoSizeColumn(i + 1);	// skip vertical headers
			width += grid.GetColumnWidth(i + 1);
		}
		auto frame = grid.GetParentFrame();
		width += GetSystemMetrics(SM_CXVSCROLL);
		grid.SetWindowPos(frame, 10, 10, width, rect.bottom-rect.top, SWP_NOZORDER | SWP_NOMOVE);
	}

private:
	template<int Col, typename Container, unsigned NumCols>
	struct PrintDataInGrid
	{
		static void Apply(int row, const Container& z, CJDGridCtrl& grid)
		{
			auto value = std::get<Col>(z[row]);

			using ValueType = decltype(value);

			CString cs;

			if constexpr (std::is_integral_v<ValueType> || std::is_floating_point_v<ValueType>)
			{
				grid.m_sortingFunctions[Col + 1] = Util::Comparison::Money;

				if( ColonesCols::template found<Col+1>() )
				{
					Util::Colones c(value);
					cs = format(c);
				}
				else if ( DollarsCols::template found<Col+1>() )
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
			grid.SetItemText(row + 1, Col + 1, cs);
			PrintDataInGrid<Col+1, Container, NumCols>::Apply(row, z, grid);
		}
	};

	template<typename Container>
	struct PrintDataInGrid<NumCols, Container, NumCols>
	{
		static void Apply(int row, const Container& z, CJDGridCtrl& grid)
		{
		}
	};

	template<typename T>
	static CString format(const T& t)
	{
		T* pT;
		auto s = typeid(T).name();
		
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
