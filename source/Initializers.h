#pragma once

#include <vector>
#include <random>

#include "Individual.h"
#include "TreeGenerators.h"

namespace Initializers
{
	std::vector<Individual> RampedHalfAndHalf(std::mt19937& random, int size, int max_levels)
	{
		std::vector<Individual> individuals;
		individuals.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			individuals.emplace_back(random, ((i / 2) * (max_levels - 2) / (size / 2)) + 2, i % 2);
		}
		return individuals;
	}
}
