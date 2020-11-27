#pragma once

#include "IDisplayer.h"

template<typename T>
class JoinedComboDisplayer : public IDisplayer
{
	using Container = std::vector<std::remove_reference_t<T>>;
	using RowType = typename Container::value_type;
	inline static constexpr size_t NumCols = std::tuple_size<RowType>::value;
	CComboBox&					box;
	Container					lines;
public:
	JoinedComboDisplayer(CComboBox& box, Container&& lines_)
		: lines{ std::move(lines_) }, box{ box}
	{
	}

	void display()
	{
		for (int i = 0; i < lines.size(); ++i)
		{
			PrintDataInCombo<Container>::Apply(i, lines, box);
		}
	}

private:
	template<typename Container>
	struct PrintDataInCombo
	{
		static void Apply(int row, const Container& z, CComboBox& box)
		{
			auto value = std::get<1>(z[row]);
			auto cs = format(value);
			int index = box.AddString (cs);
			box.SetItemData(index, std::get<0>(z[row]));
		}
	};

	template<typename T>
	static CString format(const T& t)
	{
		T* pT;
		auto s = typeid(T).name();

		return JD::to_cstring(t);
	}

	template<typename T>
	static CString format(const std::shared_ptr<T>& t)
	{
		if (t)
		{
			return JD::to_cstring(*t);
		}
		return L"";
	}
};

