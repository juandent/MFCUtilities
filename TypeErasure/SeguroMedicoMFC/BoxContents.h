#pragma once

// #include "Data_Tier.h"
#include <unordered_set>
#include "RecordLinks.h"
#include "RefIntegrity.h"
#include <cassert>

using namespace sqlite_orm;



template<typename Table>
using TableStringizer = CString(*)(Table&);

struct Posting
{
private:
	Posting() = default;
	std::unordered_set<HWND> m_postingWindows;
public:

	static Posting& get()
	{
		static Posting posting;
		return posting;
	}

	void AddWindow(HWND hwnd)
	{
		m_postingWindows.insert(hwnd);
	}

	bool exists(LPARAM lParam)
	{
		HWND hwnd = reinterpret_cast<HWND>(lParam);
		auto f = m_postingWindows.find(hwnd);
		if (f != m_postingWindows.end())
		{
			m_postingWindows.erase(hwnd);
			return true;
		}
		return false;
	}
};


template<typename Table, int Table::* keyCol, typename BoxType = CListBox>
class BoxContents
{
private:

	BoxType& m_box;
	CString(*asString)(Table& record);


	RefIntegrityManager<Table, keyCol> refIntManager;

public:

	BoxContents(BoxType& listbox, TableStringizer<Table>  f) : m_box(listbox), asString(f) {}

	template<typename ...Cols>
	std::optional<Table> insert(Cols&&... cols)
	{
		return refIntManager.insert(cols...);
	}

	//template<int Table::*pKey>
	void update(const Table& record)
	{
		refIntManager.update(record);
	}

	template<typename WhereClause> 
	std::optional<Table> exists(WhereClause& clause)
	{
		std::optional<Table> record = refIntManager.exists(clause);
		return record;
	}

	std::optional<Table> current()
	{
		std::optional<Table> record;
		int cur_sel = m_box.GetCurSel();
		if (cur_sel != npos)
		{
			auto id = static_cast<unsigned long long>(m_box.GetItemData(cur_sel));
			// DWORD_PTR
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
		if (key)
		{
			record = select(*key);
		}
		else
		{
			m_box.SetCurSel(-1);
			CString buffer;
			m_box.GetWindowTextW(buffer);
			m_box.SetWindowTextW(L"");
		}
		return record;
	}

	std::optional<Table> select(int pk)
	{
		std::optional<Table> record;
		if (pk < 0)
		{
			m_box.SetCurSel(npos);
			return record;
		}
		int index = find_index_in_listbox(pk);
		if (index != npos)
		{
			m_box.SetCurSel(index);
			// record = storage.get<Table>(pk);
			record = refIntManager.get(pk);
			// Posting::PostingWindow = m_box.m_hWnd;
				Posting::get().AddWindow(m_box.m_hWnd);
			auto wParam = (WPARAM)MAKELONG(m_box.GetDlgCtrlID(), LBN_SELCHANGE);
			m_box.GetParent()->PostMessageW(WM_COMMAND, wParam, (LPARAM)(HWND)m_box.m_hWnd);
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

	int get_pk(Table& record)
	{
		return record.*keyCol;
	}

	int get_pk(std::optional<Table>& record)
	{
		return record ? record.*keyCol : -1;
	}

	// record MUST not exist in listbox!
	int insert_into_listbox(Table& record)
	{
		int index = find_in_listbox(record);
		if (index != npos)	return index;	// exists already
		auto displayStr = asString(record);
		index = m_box.AddString(displayStr);
		m_box.SetItemData(index, get_pk(record));
		m_box.SetCurSel(index);
		return index;
	}

	bool delete_current_sel()
	{
		auto current = this->current();
		if (!current) return false;
		if (refIntManager.remove(*current))
		{
			int cur_sel = m_box.GetCurSel();
			m_box.DeleteString(cur_sel);
			return true;
		}
		return false;
	}

	void delete_from_box(Table& record)
	{
		int index = find_in_listbox(record);
		if (index == npos)	return;	// not present
		throw std::logic_error("incomplete implementation");
	}

	int find_in_listbox(Table& record)
	{
		const int pk = record.*keyCol;
		return find_index_in_listbox(pk);
	}
	template<typename T>
	void moveVectorIntoBox( T& vec)
	{
		m_box.ResetContent();
		for (auto& record : vec)
		{
			auto displayStr = asString(record);
			int index = m_box.AddString(displayStr);
			m_box.SetItemData(index, record.*keyCol);
			// SetCurSel(index);
		}
		// ???
		SetCurSel(-1);
	}
	void loadLB()
	{
		m_box.ResetContent();
		auto vec = refIntManager.getAll();

		moveVectorIntoBox(vec);
#if 0		
		for (auto& record : vec)
		{
			auto displayStr = asString(record);
			int index = m_box.AddString(displayStr);
			m_box.SetItemData(index, record.*keyCol);
			// SetCurSel(index);
		}
		// ???
		SetCurSel(-1);
#endif
	}
	template<typename whereClause>
	void loadLB(whereClause clause)
	{
		m_box.ResetContent();

		auto vec = refIntManager.getAll(clause);
		moveVectorIntoBox(vec);
#if 0
		for (auto& record : vec)
		{
			auto displayStr = asString(record);
			int index = m_box.AddString(displayStr);
			m_box.SetItemData(index, record.*keyCol);
			// SetCurSel(index);
		}
		SetCurSel(-1);
#endif
	}

	template<typename orderByClause>
	void loadLBOrderBy(orderByClause clause)
	{
		m_box.ResetContent();

		auto vec = refIntManager.getAllOrderBy(clause);
		moveVectorIntoBox(vec);
#if 0
		for (auto& record : vec)
		{
			auto displayStr = asString(record);
			int index = m_box.AddString(displayStr);
			m_box.SetItemData(index, record.*keyCol);
			// SetCurSel(index);
		}
		SetCurSel(-1);
#endif
	}

	template<typename orderByClause>
	void loadLBOrderByDesc(orderByClause clause)
	{
		m_box.ResetContent();
		auto vec = refIntManager.getAllOrderByDesc(clause);
		moveVectorIntoBox(vec);
#if 0
		for (auto& record : vec)
		{
			auto displayStr = asString(record);
			int index = m_box.AddString(displayStr);
			m_box.SetItemData(index, record.*keyCol);
			// SetCurSel(index);
		}
		SetCurSel(-1);
#endif
	}

	constexpr static const int npos = -1;
private:
	int find_index_in_listbox(const int pk)
	{
		int index = m_box.GetCount();
		while (index >= 0)
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
