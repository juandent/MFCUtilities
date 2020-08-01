#pragma once

#include "Data_Tier.h"

using namespace sqlite_orm;



template<typename Table>
using Displayer = CString(*)(Table&);



template<typename Table, int Table::*keyCol, typename BoxType = CListBox> 
class BoxContents
{
private:
	Storage::Storage_t& storage;
	BoxType& m_box;
	CString  (*displayer)(Table& record);
	
public:

	BoxContents(BoxType& listbox, Displayer<Table>  f) : storage{ Storage::getStorage() }, m_box(listbox), displayer(f) {}
	
	template<typename ...Cols>
	Table insert(Cols&&... cols)
	{
		Table record{ -1, cols... };
		record.*keyCol = storage.insert(record);
		return record;
	}

	//template<int Table::*pKey>
	void update( const Table& record )
	{
		if (record.*keyCol == -1)	return;

		storage.update(record);
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
		int cur_sel = m_box.GetCurSel();
		if (cur_sel != npos)
		{
			auto id = m_box.GetItemData(cur_sel);
			record = storage.get<Table>(id);
		}
		return record;
	}
	int GetCurSel() const
	{
		return m_box.GetCurSel();
	}
	void SetCurSel(int index) const
	{
		m_box.SetCurSel(index);
	}
	std::optional<Table> select(std::optional<int> key)
	{
		std::optional<Table> record;
		if(key)
		{
			record = select(*key);
		}
		else
		{
			m_box.SetCurSel(-1);
		}
		return record;
	}
	std::optional<Table> select(int pk)
	{
		std::optional<Table> record;
		int index = find_in_listbox(pk);
		if( index != npos )
		{
			m_box.SetCurSel(index);
			record = storage.get<Table>(pk);
			m_box.GetParent()->PostMessageW(WM_COMMAND, (WPARAM)MAKELONG(m_box.GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)(HWND)m_box.m_hWnd);
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
		auto index = m_box.AddString(displayStr);
		m_box.SetItemData(index, get_pk(record));
		m_box.SetCurSel(index);
		return index;
	}

	void delete_current_sel()
	{
		int cur_sel = m_box.GetCurSel();
		if (cur_sel != npos )
		{
			auto id = m_box.GetItemData(cur_sel);
			// m_box.DeleteString(cur_sel);
			try
			{
				Table& record = storage.get<Table>(id);
				remove(record);
			}
			catch(...)
			{
				m_box.GetParent()->MessageBoxW(L"Error borrando");
			}
			m_box.DeleteString(cur_sel);
		}
	}

	void delete_from_box(Table& record)
	{
		
	}
	
	int find_in_listbox(Table& record)
	{
		const int pk = record.*keyCol;
		return find_in_listbox(pk);
	}
	void loadLB()
	{
		m_box.ResetContent();
		auto vec = storage.get_all<Table>();

		for (auto& record : vec)
		{
			auto displayStr = displayer(record);
			int index = m_box.AddString(displayStr);
			m_box.SetItemData(index, record.*keyCol);
		}
	}
	template<typename whereClause>
	void loadLB(whereClause clause)
	{
		m_box.ResetContent();
		auto vec = storage.get_all<Table>(where(clause));

		for (auto& record : vec)
		{
			auto displayStr = displayer(record);
			int index = m_box.AddString(displayStr);
			m_box.SetItemData(index, record.*keyCol);
		}
	}
	
	constexpr static const int npos = -1;
private:
	int find_in_listbox(const int pk)
	{
		int index = m_box.GetCount();
		while ( index >= 0)
		{
			auto id = m_box.GetItemData(index);
			if (pk == id)
			{
				return index;
			}
			--index;
		}
		return npos;
	}

};
