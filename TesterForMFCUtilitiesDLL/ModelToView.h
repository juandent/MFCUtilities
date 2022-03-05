#pragma once

#include <afxwin.h>


template<typename Entity, typename TypeToDisplay>
void FillAllWithData(TypeToDisplay Entity::* pData, CComboBox& combo)
{
	auto resultset = ORM::storage.get_all<Entity>();
	for (auto& element : resultset)
	{
		combo.AddString(JD::to_cstring(element.*pData));
	}
}

template<typename Entity, typename TypeToDisplay, typename ...Args>
void FillAllWithFunc(TypeToDisplay (Entity::* pFunc)(Args...args), CComboBox& combo)
{
	auto resultset = ORM::storage.get_all<Entity>();
	for (auto& element : resultset)
	{
		combo.AddString(JD::to_cstring((element.*pFunc)(std::forward<Args>(args)...)));
	}
}

#if 0
template<typename ResultSet, typename Entity, typename TypeToDisplay, typename Widget>
void fill( ResultSet results, TypeToDisplay Entity::* pData, Widget& widget)
{
	for(auto& element : results)
	{
		//widget
	}
}
#endif
