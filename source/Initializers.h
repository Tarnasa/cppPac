#pragma once

#include <vector>
#include <random>

#include "PacmanIndividual.h"
#include "TreeGenerators.h"

namespace Initializers
{
	template<class IndividualType>
	std::vector<IndividualType> RampedHalfAndHalf(std::mt19937& random, int size, int max_levels, double parsimony_pressure)
	{
		std::vector<IndividualType> individuals;
		individuals.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			int levels = (i / 2) * (max_levels - 2) / (size / 2) + 2; // lerp from 2 to max_levels
			bool full = i % 2 == 1;
			individuals.emplace_back(random, levels, full, parsimony_pressure);
		}
		return individuals;
	}
}
