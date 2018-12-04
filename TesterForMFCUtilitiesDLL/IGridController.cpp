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
