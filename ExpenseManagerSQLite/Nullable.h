#pragma once


struct Nullable
{
	template<typename T>
	using Type = std::shared_ptr<T>;

	template<typename T>
	inline
		static Type<T> make_nullable(const T& t)
	{
		return std::make_shared<T>(t);
	}

	template<typename Data, typename...Fks>
	inline
		static
		Type<Data> getFromFKs(Fks...fks)
	{
		if ((fks && ...))
		{
			auto instance = Storage::getStorage().get<Data>(*fks...);
			return make_nullable(instance);
		}
		return nullptr;
	}
};

