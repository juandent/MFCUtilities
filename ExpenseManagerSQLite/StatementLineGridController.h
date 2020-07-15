#pragma once


#include "TesterForMFCUtilitiesDLL/IGridController.h"
#include "JDGridCtrl/JDGridCtrl.h"


struct Translation
{
	date::sys_days statement_date;
	std::string    origin_account;
	date::sys_days line_date;
	std::string    concepto;
	double			amount_local;
	double			amount_dollars;
	bool			is_enabled;
	std::string     category_name;
	std::string     description;
	std::string		owner_name;
};









class StatementLineGridController :     public IGridController
{
	friend class ConceptsAndAccounts;
	std::vector<Translation> lines;
	enum Columns { STMT_DATE=1, ORIGIN_ACCOUNT, LINE_DATE, CONCEPTO, AMOUNT_LOCAL, AMOUNT_DOLLARS, IS_ENABLED, CATEGORY_NAME, DESCRIPCION, OWNER_NAME, NUM_COLUMNS = OWNER_NAME};

public:
	StatementLineGridController(CJDGridCtrl& grid, int count_columns) : IGridController(grid, count_columns) {}

	virtual void HandleGridClick(NM_GRIDVIEW* pItem) override;
	virtual void FillHeaders() override;
	virtual void FillLine(int row) override;
	virtual void SetSorting() override;
public:

	virtual void OnInitialUpdate() override;
	virtual void RefreshGrid() override ;

 };

