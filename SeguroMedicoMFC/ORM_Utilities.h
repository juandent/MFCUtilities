#pragma once


enum class Operation { doUpdate = 1, doInsert = 2 };


template<typename Table>
std::optional<Table> getCurrent(CEdit& editCtrl)
{
	using namespace sqlite_orm;

	CString rId;
	editCtrl.GetWindowTextW(rId);
	if (rId.IsEmpty())
	{
		return std::nullopt;
	}
	auto id_str = Util::from_cstring(rId);
	auto id = std::stoi(id_str);
	auto record = Storage::getStorage().get_optional < Table>(id);
	///////////	
	return record;
}

template<typename Table>
Operation whatOperation(CEdit& editCtrl)
{
	auto record = getCurrent<Table>(editCtrl);
	return record ? Operation::doUpdate : Operation::doInsert;
}

template<typename Table>
void setIdFromRecord(CEdit& editCtrl, int pk)
{
	editCtrl.SetWindowTextW(Util::to_cstring(pk));
}

template<typename Data>
void SetText(CEdit& edit_box, Data data)
{
	edit_box.SetWindowTextW(Util::to_cstring(data));
}

inline void NullText(CEdit& edit_box)
{
	edit_box.SetWindowTextW(L"");
}

inline void SetColones(CEdit& edit_box, double amount)
{
	Util::Colones col{ amount };
	auto col_str = Util::to_string(col);
	auto str = Util::to_cstring(col_str);
	edit_box.SetWindowTextW(str);
}

inline void SetAmount(CEdit& edit_box, double amount)
{
	auto str = Util::to_cstring(amount);
	edit_box.SetWindowTextW(str);
}

inline void SetLongLong(CEdit& edit_box, long long amount)
{
	auto str = Util::to_cstring(amount);
	edit_box.SetWindowTextW(str);
}


template<typename Table, int Table::* key>
struct IdManager
{
	CEdit& editCtrl;
	std::optional<Table>& record;
	IdManager(CEdit& editCtrl, std::optional<Table>& record)
		: editCtrl(editCtrl), record{ record }
	{}
	void SetId()
	{
		editCtrl.SetWindowTextW(Util::to_cstring(record->*key));
	}
	std::optional<Table> GetIdToCurrent()
	{
		record = getCurrent<Table>(editCtrl);
	}
};


inline void SetDate(CDateTimeCtrl& ctrl, date::sys_days sysdate)
{
	auto date = Util::to_ole_date_time(sysdate);
	ctrl.SetTime(date);
}

inline date::sys_days GetDate(CDateTimeCtrl& ctrl)
{
	COleDateTime rOleDateTime;
	ctrl.GetTime(rOleDateTime);
	date::sys_days date = Util::to_sys_days(rOleDateTime);
	return date;
}

inline std::string GetText(CEdit& ctrl)
{
	CString str;
	ctrl.GetWindowTextW(str);
	if (str.IsEmpty())
		return ""s;
	auto s = Util::from_cstring(str);
	return s;
}

inline double GetAmount(CEdit& ctrl)
{
	auto str = GetText(ctrl);
	if (str.empty())	return 0.0;
	return std::stold(str);
}


inline int GetInteger(CEdit& ctrl)
{
	auto str = GetText(ctrl);
	if (str.empty())	return 0.0;
	return std::stoi(str);
}

inline long GetLong(CEdit& ctrl)
{
	auto str = GetText(ctrl);
	if (str.empty())	return 0.0;
	return std::stol(str);
}

inline long long GetLongLong(CEdit& ctrl)
{
	auto str = GetText(ctrl);
	if (str.empty())	return 0.0;
	return std::stoll(str);
}

