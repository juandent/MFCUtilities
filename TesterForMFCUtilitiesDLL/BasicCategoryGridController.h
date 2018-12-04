#pragma once
#include "IGridController.h"
#include "ORM/ORM.Repository.h"


class BasicCategoryGridController : public IGridController
{
	std::vector<Model::Category> lines;
	enum Columns { NAME = 1, REAL = 2, NUM_COLUMNS};
	virtual void OnInitialUpdate(int num_rows) override;
public:
	BasicCategoryGridController(CJDGridCtrl& grid) : IGridController{ grid, Columns::NUM_COLUMNS } {};
	
	BasicCategoryGridController(const BasicCategoryGridController&) = delete;

	~BasicCategoryGridController() {}

	// Inherited via IGridController
	virtual void OnInitialUpdate() override;

	virtual void OnUpdate() override;

	virtual void OnGridClick(NMHDR * pNotifyStruct) override;

	virtual void FillHeaders() override;

	virtual void FillGrid() override;

	virtual void FillLine(int row) override;

};

