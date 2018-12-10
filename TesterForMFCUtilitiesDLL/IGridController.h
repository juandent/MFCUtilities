#pragma once

#include <JDGridCtrl.h>


class IGridController
{
protected:
	CJDGridCtrl& grid;
	const int count_columns;

	IGridController(CJDGridCtrl& grid, int count_columns) : grid{ grid }, count_columns{count_columns} {}
	
	void DoInitialUpdate(int num_rows);
	void AutoSizeColumns();
	void FillGrid(int num_rows);

	virtual void HandleGridClick(NM_GRIDVIEW* pItem) = 0;
	virtual void FillHeaders() = 0;
	virtual void FillLine(int row) = 0;
	virtual void SetSorting() = 0;
public:
	virtual ~IGridController() {}

	virtual void OnInitialUpdate() = 0;
	virtual void RefreshGrid() = 0;

	void OnGridClick(NMHDR *pNotifyStruct);
};

