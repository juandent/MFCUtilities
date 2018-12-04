#include "stdafx.h"
#include "IGridController.h"

void IGridController::DoInitialUpdate(int num_rows)
{
	grid.SetRowCount(num_rows + 1);
	grid.SetColumnCount(count_columns);
	grid.SetFixedRowCount();
	grid.SetFixedColumnCount();
	grid.SetHeaderSort(true);
	grid.SetSingleRowSelection(true);

	FillHeaders();

	RefreshGrid();

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

void IGridController::OnGridClick(NMHDR * pNotifyStruct)
{
	NM_GRIDVIEW* pItem = reinterpret_cast<NM_GRIDVIEW*>(pNotifyStruct);
	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	if (pItem->hdr.idFrom == grid.GetDlgCtrlID())
	{
		//pItem->hdr.hwndFrom;
		HandleGridClick(pItem);
	}
}

