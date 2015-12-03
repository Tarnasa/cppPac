#pragma once

#include <vector>
#include <random>

#include "PacmanIndividual.h"
#include "TreeGenerators.h"

namespace Initializers
{
	template<class IndividualType>
	std::vector<IndividualType> RampedHalfAndHalf(std::mt19937& random, int size, int max_levels)
	{
		std::vector<IndividualType> individuals;
		individuals.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			individuals.emplace_back(random, ((i / 2) * (max_levels - 2) / (size / 2)) + 2, i % 2);
		}
		return individuals;
	}
}
