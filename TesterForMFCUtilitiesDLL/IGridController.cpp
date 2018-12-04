#include "stdafx.h"
#include "IGridController.h"

void IGridController::OnInitialUpdate(int num_rows)
{
	grid.SetRowCount(num_rows);
	grid.SetColumnCount(count_columns);
	grid.SetFixedRowCount();
	grid.SetFixedColumnCount();
	grid.SetHeaderSort(true);
	grid.SetSingleRowSelection(true);

	FillHeaders();

	OnUpdate();

	AutoSizeColumns();
}

void IGridController::AutoSizeColumns()
{
	for (int i = 0; i < count_columns; ++i)
	{
		grid.AutoSizeColumn(i);
	}
}

void IGridController::FillGrid(int num_rows)
{
	for (int i = 0; i < num_rows; ++i)
	{
		FillLine(i);
	}
}

