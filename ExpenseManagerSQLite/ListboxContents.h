#pragma once

#include "Data_Tier.h"

using namespace sqlite_orm;



template<typename Table>
using Displayer = CString(*)(Table&);



template<typename Table, typename int Table::*keyCol> 
class ListboxContents
{
private:
	Storage::Storage_t& storage;
	CListBox& m_listbox;
	CString  (*displayer)(Table& record);
	
public:

	ListboxContents(CListBox& listbox, Displayer<Table>  f) : storage{ Storage::getStorage() }, m_listbox(listbox), displayer(f) {}
	
	template<typename ...Cols>
	Table insert(Cols&&... cols)
	{
		Table record{ -1, cols... };
		record.*keyCol = storage.insert(record);
		return record;
	}

	template<typename WhereClause, typename ...Cols>
	std::optional<Table> exists( WhereClause& clause, Cols&& ... cols )
	{
		std::optional<Table> record;
		auto e = storage.select( columns(cols...), where(clause));
		if(e.size() > 0)
		{
			auto id = std::get<0>(e[0]);
			record = storage.get<Table>(id);
		}
		
		return record;
	}

	std::optional<Table> current()
	{
		std::optional<Table> record;
		int cur_sel = m_listbox.GetCurSel();
		if (cur_sel != npos)
		{
			auto id = m_listbox.GetItemData(cur_sel);
			record = storage.get<Table>(id);
		}
		return record;
	}

	std::optional<Table> select(int pk)
	{
		std::optional<Table> record;
		int index = find_in_listbox(pk);
		if( index != npos )
		{
			m_listbox.SetCurSel(index);
			record = storage.get<Table>(pk);
			m_listbox.GetParent()->PostMessageW(WM_COMMAND, (WPARAM)MAKELONG(m_listbox.GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)(HWND)m_listbox.m_hWnd);
		}
		return record;
	}

	void remove(Table& record)
	{
		assert(record.*keyCol > -1);
		storage.remove<Table>(get_pk(record));
	}

	int get_pk( Table& record)
	{
		return record.*keyCol;
	}

	// record MUST not exist in listbox!
	int insert_into_listbox( Table& record)
	{
		assert( get_pk(record) != npos);
		auto displayStr = displayer(record);
		auto index = m_listbox.AddString(displayStr);
		m_listbox.SetItemData(index, get_pk(record));
		return index;
	}

	void delete_current_sel()
	{
		int cur_sel = m_listbox.GetCurSel();
		if (cur_sel != npos )
		{
			auto id = m_listbox.GetItemData(cur_sel);
			m_listbox.DeleteString(cur_sel);
			Table& record = storage.get<Table>(id);
			remove(record);
		}
	}

	void delete_from_listbox(Table& record)
	{
		
	}
	
	int find_in_listbox(Table& record)
	{
		const int pk = record.*keyCol;
		return find_in_listbox(pk);
	}
	void loadLB()
	{
		auto vec = storage.get_all<Table>();

		for (auto& record : vec)
		{
			auto displayStr = displayer(record);
			int index = m_listbox.AddString(displayStr);
			m_listbox.SetItemData(index, record.*keyCol);
		}
	}
	constexpr static const int npos = -1;
private:
	int find_in_listbox(const int pk)
	{
		int index = m_listbox.GetCount();
		while ( index >= 0)
		{
			auto id = m_listbox.GetItemData(index);
			if (pk == id)
			{
				return index;
			}
			--index;
		}
		return npos;
	}

};
