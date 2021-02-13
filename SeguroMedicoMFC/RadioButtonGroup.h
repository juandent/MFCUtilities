#pragma once

#include "framework.h"

class RadioButtonGroup
{
	std::vector<CButton*> m_radios;
public:
	RadioButtonGroup( std::initializer_list<CButton*> list )
		: m_radios{list}
	{
	}
	void Initialize()
	{
		m_radios[0]->SetCheck(BST_CHECKED);
	}
	int get_value() const
	{
		for(int idx = 0; idx < m_radios.size(); ++idx)
		{
			if (m_radios[idx]->GetCheck())
				return idx;
		}
		throw std::logic_error("There must be 1 selected radio button in a group");
	}
	void set_value(int idx_select)
	{
		for (int idx = 0; idx < m_radios.size(); ++idx)
		{
			m_radios[idx]->SetCheck(idx == idx_select);
		}
	}
};






