/// Misc convenience functions

#pragma once

#include <algorithm>
#include <functional>
#include <vector>
#include <stdarg.h>
#include <cstdio>

template<class ContainerType, class PredicateType>
bool any(const ContainerType& container, const PredicateType& predicate)
{
	return std::any_of(container.cbegin(), container.cend(), predicate);
}

template<class ElementType, class PredicateType>
std::vector<typename std::result_of<PredicateType(ElementType)>::type>
	map(std::vector<ElementType>& container, const PredicateType& predicate)
{
	using result_type = typename std::result_of<PredicateType(ElementType)>::type;
	std::vector<result_type> result;
	result.reserve(container.size());
	for (auto&& e : container)
	{
		result.emplace_back(predicate(e));
	}
	return result;
}

template<typename T>
T clamp(T low, T high, T input)
{
	return input > low ? (input < high ? input : high) : low;
}

int get_length_int(int n);

int get_printf_length(int max_length, const char* format, ...);
