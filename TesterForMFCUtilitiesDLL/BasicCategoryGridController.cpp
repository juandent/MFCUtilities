#include "stdafx.h"
#include "BasicCategoryGridController.h"



using namespace sqlite_orm;

void BasicCategoryGridController::OnInitialUpdate()
{
	grid.SetColumnCount(Columns::NUM_COLUMNS);
	grid.SetFixedRowCount();
	grid.SetFixedColumnCount();
	grid.SetHeaderSort(true);
	grid.m_sortingFunctions = { JD::Comparison::Text, JD::Comparison::Text };

	grid.SetSingleRowSelection(true);

	/////////

	lines = ORM::Storage::getStorage().get_all<Model::Category>(order_by(&Model::Category::m_name_id));

	// lines = repository.allCategoriesSortedBy(order_by(&Model::Category::m_name_id));

	grid.SetRowCount(lines.size() + 1);

	grid.AutoSizeColumn(Columns::NAME);
	grid.AutoSizeColumn(Columns::REAL);

}

void BasicCategoryGridController::OnUpdate()
{
	FillGrid();
}

void BasicCategoryGridController::OnGridClick(NMHDR * pNotifyStruct)
{
	NM_GRIDVIEW* pItem = reinterpret_cast<NM_GRIDVIEW*>(pNotifyStruct);
	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	if (pItem->hdr.idFrom == grid.GetDlgCtrlID())
	{
		//pItem->hdr.hwndFrom;
		
	}
}

void BasicCategoryGridController::FillHeaders()
{
	grid.SetItemText(0, Columns::NAME, L"CATEGORY NAME");
	grid.SetItemText(0, Columns::REAL, L"REAL?");
}

void BasicCategoryGridController::FillGrid()
{
	for (int i = 0; i < lines.size(); ++i)
	{
		FillLine(i);
	}
}

void BasicCategoryGridController::FillLine(int row)
{
	auto line = lines[row];

	auto txt = JD::to_cstring(line.m_name_id);
	grid.SetItemText(row + 1, Columns::NAME, txt);
	auto boolVal = JD::to_cstring(line.m_real_expense_or_income ? "Real" : "Not real");
	grid.SetItemText(row + 1, Columns::REAL, boolVal);

}
