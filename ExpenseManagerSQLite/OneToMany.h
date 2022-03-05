#pragma once

#include "BoxContents.h"

template<typename Table1, int Table1::* keyCol, typename TableN, int TableN::* foreignKeyCol, int TableN::* nKeyCol >
class OneToMany
{
public:
	template<typename Table>
	using Stringizing = CString(*)(Table&);
	
	OneToMany(CComboBox& side1, Stringizing<Table1> f1, CListBox& sideN, Stringizing<TableN> fN)
		: m_box1(side1, f1), m_boxN(sideN, fN)
	{}

	BoxContents < Table1, keyCol, CComboBox> m_box1;
	BoxContents < TableN, nKeyCol> m_boxN;

	void On1SelectionChanged()
	{
		auto rec = m_box1.current();

		if (!rec)	return;
		
		Table1 current = *rec;

		auto whereClause = (c(foreignKeyCol) 
			
			== current.*keyCol);

		m_boxN.loadLB(whereClause);
	}
	void OnInitDialog()
	{
		m_box1.loadLB();
		m_boxN.loadLB();
	}
	std::optional<TableN> OnNSelectionChanged()
	{
		if (!m_box1.current())  return std::nullopt;
		auto rec = m_boxN.current();
		return rec;
	}
};

