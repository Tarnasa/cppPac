/// Misc convenience functions

#pragma once

#include <algorithm>
#include <functional>
#include <vector>
#include <stdarg.h>
#include <cstdio>
#include <random>

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

template<typename T>
const T& choice(std::mt19937& random, const std::vector<T>& container)
{
	return container[std::uniform_int_distribution<int>(0, container.size() - 1)(random)];
}

// Non-const version
template<typename T>
T& choice(std::mt19937& random, std::vector<T>& container)
{
	return container[std::uniform_int_distribution<int>(0, container.size() - 1)(random)];
}

int random_int(std::mt19937& random, int min, int max);

bool chance(std::mt19937& random, double chance);

std::string replace(const std::string& s, const std::string& search, const std::string& replacement);

template<class ElementType, class PredicateType>
double average(const std::vector<ElementType>& list, const PredicateType& predicate)
{
	double sum = 0;
	for (auto&& e : list)
	{
		sum += predicate(e);
	}
	return sum / list.size();
}
