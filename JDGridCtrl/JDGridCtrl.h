#pragma once

#include "GridCtrl.h"

// CJDGridCtrl

class GRIDCTRL_API CJDGridCtrl : public CGridCtrl
{
	DECLARE_DYNAMIC(CJDGridCtrl)

public:
	CJDGridCtrl(int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
	virtual ~CJDGridCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	TOOLINFO m_ti;
	BOOL m_tracking = false;
	CToolTipCtrl m_Tip;
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow() override;
	void ScrollDownNRows(int num_rows);
	void ScrollUpNRows(int num_rows);
	int RowNumberForFirstGridLine();
	void SetSelectAndScroll(int row_to_select);
	void Scroll(int row_to_select);
	void SelectRow(int row_to_select);
};


