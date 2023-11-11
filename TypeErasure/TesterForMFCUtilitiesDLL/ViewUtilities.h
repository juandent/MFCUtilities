#pragma once



inline 
std::pair<bool, std::string> get_selection(const CButton& button, const CComboBox& combo)
{
	if (button.GetCheck())
	{
		int sel = combo.GetCurSel();
		CString selection;
		combo.GetLBText(sel, selection);
		auto res = JD::to_string(selection.GetBuffer());
		return std::make_pair(true, res);
	}
	return std::make_pair(false,"");
}


inline
bool set_selection(const std::pair<bool, std::string>& p, CButton& button, CComboBox& combo)
{
	if( p.first)
	{
		button.SetCheck(true);
		CString str = JD::to_cstring(p.second);
		combo.SelectString(0, str);
	}
	return p.first;
}

