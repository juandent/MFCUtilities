#pragma once

#include "Data_Tier.h"
#include "RecordLinks.h"
#include "RefIntegrity.h"

using namespace sqlite_orm;



template<typename Table>
using TableStringizer = CString(*)(Table&);



template<typename Table, int Table::*keyCol, typename BoxType = CListBox> 
class BoxContents
{
private:
//	Storage::Storage_t& storage;
	BoxType& m_box;
	CString  (*asString)(Table& record);


	RefIntegrityManager<Table, keyCol> refIntManager;
	
public:

	BoxContents(BoxType& listbox, TableStringizer<Table>  f) : m_box(listbox), asString(f) {}
	
	template<typename ...Cols>
	Table insert(Cols&&... cols)
	{
		return refIntManager.insert(cols...);
		// Table record{ -1, cols... };
		// record.*keyCol = storage.insert(record);
		// return record;
	}

	//template<int Table::*pKey>
	void update( const Table& record )
	{
		refIntManager.update(record);
		// if (record.*keyCol == -1)	return;
		//
		// storage.update(record);
	}

	template<typename WhereClause, typename ...Cols>
	std::optional<Table> exists( WhereClause& clause, Cols&& ... cols )
	{
		std::optional<Table> record = refIntManager.exists(clause, cols...);
		return record;
		
		// auto e = storage.select( columns(cols...), where(clause));
		// if(e.size() > 0)
		// {
		// 	auto id = std::get<0>(e[0]);
		// 	record = storage.get<Table>(id);
		// }
		//
		// return record;
	}

	std::optional<Table> current()
	{
		std::optional<Table> record;
		int cur_sel = m_box.GetCurSel();
		if (cur_sel != npos)
		{
			auto id = m_box.GetItemData(cur_sel);
			// record = storage.get<Table>(id);
			record = refIntManager.get(id);
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
		if(pk < 0)
		{
			m_box.SetCurSel(npos);
			return record;
		}
		int index = find_in_listbox(pk);
		if( index != npos )
		{
			m_box.SetCurSel(index);
			// record = storage.get<Table>(pk);
			record = refIntManager.get(pk);
			m_box.GetParent()->PostMessageW(WM_COMMAND, (WPARAM)MAKELONG(m_box.GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)(HWND)m_box.m_hWnd);
		}
		return record;
	}

	void remove(Table& record)
	{
		refIntManager.remove(record);
		// assert(record.*keyCol > -1);
		// if (!RefIntegrity::canDelete(*current))
		// 	return;
		//
		// storage.remove<Table>(get_pk(record));
	}

	int get_pk( Table& record)
	{
		return record.*keyCol;
	}

	// record MUST not exist in listbox!
	int insert_into_listbox( Table& record)
	{
		assert( get_pk(record) != npos);
		auto displayStr = asString(record);
		auto index = m_box.AddString(displayStr);
		m_box.SetItemData(index, get_pk(record));
		m_box.SetCurSel(index);
		return index;
	}

	bool delete_current_sel()
	{
		auto current = this->current();
		if (!current) return false;
		if( refIntManager.remove(*current))
		{
			int cur_sel = m_box.GetCurSel();
			m_box.DeleteString(cur_sel);
			return true;
		}
// #if 0
// 		bool has_links = RecordLinks::has_links(*current);
// 		if (has_links) return false;
// #else
// 		if (! RefIntegrity::canDelete(*current))
// 			return false;
// #endif

		// int cur_sel = m_box.GetCurSel();
		// // remove(*current);
		// m_box.DeleteString(cur_sel);
		// return true;
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
		// auto vec = storage.get_all<Table>();
		auto vec = refIntManager.getAll();
		
		for (auto& record : vec)
		{
			auto displayStr = asString(record);
			int index = m_box.AddString(displayStr);
			m_box.SetItemData(index, record.*keyCol);
		}
	}
	template<typename whereClause>
	void loadLB(whereClause clause)
	{
		m_box.ResetContent();
		// auto vec = storage.get_all<Table>(where(clause));
		auto vec = refIntManager.getAll(clause);
		for (auto& record : vec)
		{
			auto displayStr = asString(record);
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
