﻿#pragma once



template<typename T, int MaxCol>
class JoinedGridDisplayer
{
	using Container = std::vector<std::remove_reference_t<T>>;
	using RowType = typename Container::value_type;
	// next cannot be declared constexpr - why??
	inline static size_t NumCols = std::tuple_size<RowType>::value;
	CJDGridCtrl&				grid;
	Container					lines;
	std::vector<std::string>	headers;
public:
	JoinedGridDisplayer(CJDGridCtrl& grid,
		Container&& lines_,
		std::vector<std::string>&& headers_)
		: lines{ std::move(lines_) }, grid{ grid }, headers{ std::move(headers_) }
	{
		grid.SetColumnCount(headers.size() + 1);
		grid.SetRowCount(lines.size() + 1);
		grid.SetFixedRowCount();
		grid.SetFixedColumnCount();
		grid.SetHeaderSort(true);
		grid.SetSingleRowSelection(true);

		int col = 1;
		for (auto& str : headers)
		{
			auto head = JD::to_cstring(str);
			grid.SetItemText(0, col, head);
			++col;
		}
		//static_assert(NumCols == MaxCol, "");
	}

	void display()
	{
		for (int i = 0; i < lines.size(); ++i)
		{
			PrintDataInGrid<0, Container>::Apply(i, lines, grid);
		}
		for (int i = 0; i < headers.size(); ++i)
		{
			grid.AutoSizeColumn(i + 1);	// skip vertical headers
		}
	}

private:
	template<int Col, typename Container>
	struct PrintDataInGrid
	{
		static void Apply(int row, const Container& z, CJDGridCtrl& grid)
		{
			auto value = std::get<Col>(z[row]);
			auto cs = format(value);
			grid.SetItemText(row + 1, Col + 1, cs);
			PrintDataInGrid<Col+1, Container>::Apply(row, z, grid);
		}
	};

	template<typename Container>
	struct PrintDataInGrid<MaxCol, Container>
	{
		static void Apply(int row, const Container& z, CJDGridCtrl& grid)
		{
		}
	};

	template<typename T>
	static CString format(const T& t)
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