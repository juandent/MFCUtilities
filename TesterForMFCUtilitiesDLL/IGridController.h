#pragma once

#include <JDGridCtrl.h>


class IGridController
{
protected:
	CJDGridCtrl& grid;
	IGridController(CJDGridCtrl& grid) : grid{ grid } {}
public:
	virtual ~IGridController() {}

	virtual void OnInitialUpdate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnGridClick(NMHDR *pNotifyStruct) = 0;
	virtual void FillHeaders() = 0;
	virtual void FillGrid() = 0;
	virtual void FillLine(int row) = 0;

};

