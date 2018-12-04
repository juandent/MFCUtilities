#include "stdafx.h"

#include "BasicCategoryGridController.h"

#include "GridCellCheck.h"

using namespace sqlite_orm;


void BasicCategoryGridController::OnInitialUpdate()
{
	lines = ORM::Storage::getStorage().get_all<Model::Category>(order_by(&Model::Category::m_name_id));
	DoInitialUpdate(lines.size());
	grid.m_sortingFunctions = { JD::Comparison::Text, JD::Comparison::Text, JD::Comparison::Text};
}

void BasicCategoryGridController::RefreshGrid()
{
	FillGrid( lines.size());
}

void BasicCategoryGridController::HandleGridClick(NM_GRIDVIEW * pItem)
{
	if (pItem->iRow == 0 && pItem->iColumn > 0)	// header
	{
		int sortCol = grid.GetSortColumn();
		auto sortIsAsc = grid.GetSortAscending();

		// sort
		switch (pItem->iColumn)
		{
		case Columns::NAME:
			break;
		case Columns::REAL:
			break;
		}
	}
}

void BasicCategoryGridController::FillHeaders()
{
	grid.SetItemText(0, Columns::NAME, L"CATEGORY NAME");
	grid.SetItemText(0, Columns::REAL, L"REAL?");
}

void BasicCategoryGridController::FillLine(int row)
{
	auto line = lines[row];

	auto txt = JD::to_cstring(line.m_name_id);
	grid.SetItemText(row + 1, Columns::NAME, txt);


	grid.SetCellType(row + 1, Columns::REAL, RUNTIME_CLASS(CGridCellCheck));
	auto realCell = dynamic_cast<CGridCellCheck*>(grid.GetCell(row + 1, Columns::REAL));
	realCell->SetCheck(line.m_real_expense_or_income);
}
