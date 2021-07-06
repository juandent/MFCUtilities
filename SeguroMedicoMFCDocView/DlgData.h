#pragma once

namespace string_conversions
{
	inline std::string to_string(const std::wstring& str, const std::locale& loc = std::locale{})
	{
		std::vector<char> buf(str.size());
		std::use_facet<std::ctype<wchar_t>>(loc).narrow(str.data(), str.data() + str.size(), '?', buf.data());

		return std::string(buf.data(), buf.size());
	}

	inline std::string from_cstring(const CString& msg)
	{
		auto m = static_cast<LPCTSTR>(msg);
		auto str = to_string(m);
		return str;
	}
	inline std::wstring to_wstring(const std::string& str, const std::locale& loc = std::locale{})
	{
		std::vector<wchar_t> buf(str.size());
		std::use_facet<std::ctype<wchar_t>>(loc).widen(str.data(), str.data() + str.size(), buf.data());

		return std::wstring(buf.data(), buf.size());
	}

	inline CString to_cstring(std::string msg)
	{
		std::wstring message = to_wstring(msg);
		CString msg_as_cstring{ message.c_str() };
		return msg_as_cstring;
	}
}

inline static void DDX_Text(CDataExchange* pDX, int nIDC, std::string& value)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	CString cs_value;
	if (pDX->m_bSaveAndValidate)
	{
		int nLen = ::GetWindowTextLength(hWndCtrl);
		::GetWindowText(hWndCtrl, cs_value.GetBufferSetLength(nLen), nLen + 1);
		cs_value.ReleaseBuffer();
		value = string_conversions::from_cstring(cs_value);
	}
	else
	{
		cs_value = string_conversions::to_cstring(value);
		AfxSetWindowText(hWndCtrl, cs_value);
	}
}

// 
inline static void DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, std::chrono::sys_days& std_date)
{
	using namespace std::chrono;
	
	COleDateTime value;

	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	CDateTimeCtrl* pWnd = (CDateTimeCtrl*)CWnd::FromHandle(hWndCtrl);
	ENSURE(pWnd);
	if (pDX->m_bSaveAndValidate)
	{
		pWnd->GetTime(value);

		year_month_day ymd{ static_cast<year>(value.GetYear()), static_cast<month>(value.GetMonth()), static_cast<day>(value.GetDay()) };
		std_date = ymd;
	}
	else
	{
		year_month_day ymd{ std_date };
		// value.SetDate(static_cast<int>(ymd.year()), static_cast<unsigned int>(ymd.month()), static_cast<unsigned int>(ymd.day()));
		value.SetDateTime(static_cast<int>(ymd.year()), static_cast<unsigned int>(ymd.month()), static_cast<unsigned int>(ymd.day()), 0, 0, 0);
		pWnd->SetTime(value);
	}
}

inline CEdit& operator<<(CEdit& edit, std::string& cs)
{
	auto s= string_conversions::to_cstring(cs);
	SetText(edit, s);
	return edit;
}
