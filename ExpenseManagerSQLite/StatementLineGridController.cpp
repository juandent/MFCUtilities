#include "stdafx.h"
#include "StatementLineGridController.h"


void StatementLineGridController::HandleGridClick(NM_GRIDVIEW* pItem)
{
	if(::OpenClipboard(grid.m_hWnd))
	{
		::EmptyClipboard();

		auto csText = grid.GetItemText(pItem->iRow, pItem->iColumn);

		HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, lstrlen(csText) + 1);
		LPWSTR pData = (LPWSTR) ::GlobalLock(hData);
		lstrcpy(pData, csText);
		::GlobalUnlock(hData);

		::SetClipboardData(CF_UNICODETEXT, hData);
		::CloseClipboard();
	}
}

void StatementLineGridController::FillHeaders()
{
	grid.SetItemText(0, Columns::STMT_DATE, L"STMT DATE");
	grid.SetItemText(0, Columns::ORIGIN_ACCOUNT, L"ORIGIN_ACCOUNT");
	grid.SetItemText(0, Columns::LINE_DATE, L"LINE_DATE");
	grid.SetItemText(0, Columns::CONCEPTO, L"CONCEPTO");
	grid.SetItemText(0, Columns::AMOUNT_LOCAL, L"AMOUNT_LOCAL");
	grid.SetItemText(0, Columns::AMOUNT_DOLLARS, L"AMOUNT_DOLLARS");
	grid.SetItemText(0, Columns::IS_ENABLED, L"IS_ENABLED");
	grid.SetItemText(0, Columns::CATEGORY_NAME, L"CATEGORY_NAME");
	grid.SetItemText(0, Columns::DESCRIPCION, L"DESCRIPCION");
	grid.SetItemText(0, Columns::OWNER_NAME, L"OWNER_NAME");

}

void StatementLineGridController::FillLine(int row)
{
	auto line = lines[row++];
	
	grid.SetItemText(row, Columns::STMT_DATE, JD::to_cstring(line.statement_date));
	grid.SetItemText(row, Columns::ORIGIN_ACCOUNT, JD::to_cstring( line.origin_account));
	grid.SetItemText(row, Columns::LINE_DATE, JD::to_cstring(line.line_date));
	grid.SetItemText(row, Columns::CONCEPTO, JD::to_cstring(line.concepto));


	double local = line.amount_local;
	Colones c{ local };
	grid.SetItemText(row, Columns::AMOUNT_LOCAL, JD::to_cstring(c));

	double dolar = line.amount_dollars;
	Dolares d{ dolar };
	grid.SetItemText(row, Columns::AMOUNT_DOLLARS, JD::to_cstring(d));
	
	grid.SetItemText(row, Columns::IS_ENABLED, JD::to_cstring(line.is_enabled));
	grid.SetItemText(row, Columns::CATEGORY_NAME, JD::to_cstring(line.category_name));
	grid.SetItemText(row, Columns::DESCRIPCION, JD::to_cstring(line.description));
	grid.SetItemText(row, Columns::OWNER_NAME, JD::to_cstring(line.owner_name));

}

void StatementLineGridController::SetSorting()
{
}

void StatementLineGridController::OnInitialUpdate()
{
	DoInitialUpdate(lines.size());
}

void StatementLineGridController::RefreshGrid()
{
	FillGrid(lines.size());
}


