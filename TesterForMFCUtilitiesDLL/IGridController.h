#pragma once

#include <JDGridCtrl.h>


class IGridController
{
protected:
	CJDGridCtrl& grid;
	const int count_columns;
	IGridController(CJDGridCtrl& grid, int count_columns) : grid{ grid }, count_columns{count_columns} {}
public:
	virtual ~IGridController() {}

	virtual void OnInitialUpdate(int num_rows);
	virtual void OnUpdate() = 0;
	virtual void OnGridClick(NMHDR *pNotifyStruct) = 0;
	virtual void FillHeaders() = 0;
	virtual void FillGrid() = 0;
	virtual void FillLine(int row) = 0;
	virtual void AutoSizeColumns() = 0;

};

