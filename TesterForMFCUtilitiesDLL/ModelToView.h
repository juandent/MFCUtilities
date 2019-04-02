#pragma once

#include <afxwin.h>


template<typename Entity, typename TypeToDisplay>
void FillAll(TypeToDisplay Entity::* pData, CComboBox& combo)
{
	auto resultset = ORM::Storage::getStorage().get_all<Entity>();
	for (auto& element : resultset)
	{
		combo.AddString(JD::to_cstring(element.*pData));
	}
}

