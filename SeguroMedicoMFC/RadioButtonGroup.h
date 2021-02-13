#pragma once

#include "framework.h"

class RadioButtonGroup
{
	std::vector<CButton*> m_radios;
public:
	RadioButtonGroup( std::initializer_list<CButton*> list )
		: m_radios{list}
	{}

	int get_value() const
	{
		for(int idx = 0; idx < m_radios.size(); ++idx)
		{
			if (m_radios[idx]->GetCheck())
				return idx;
		}
		throw std::logic_error("There must be 1 selected radio button in a group");
	}
};






