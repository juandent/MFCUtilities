#pragma once

#include <vector>

template<typename T>
class Collection
{
	std::vector<T> m_elements;
public:
	Collection(std::vector<T>&& elements ) : m_elements{std::move(elements)} {}

	virtual void generate() = 0;
};