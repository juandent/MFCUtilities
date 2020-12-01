#pragma once


struct IDisplayer
{
	virtual void display() = 0;
};

namespace Util
{
	inline CString to_cstring(const std::optional<int>& o)
	{
		if (o)
		{
			return to_cstring(*o);
		}
		else
		{
			return CString{ "" };
		}
	}
}
