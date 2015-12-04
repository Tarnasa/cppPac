#pragma once

#include <vector>
#include <random>

#include "PacmanIndividual.h"

namespace Survival
{
	template<class IndividualType>
	void truncate(std::vector<IndividualType>& individuals, int number_to_select)
	{
		individuals.erase(individuals.begin() + number_to_select, individuals.begin() + individuals.size());
	}

	template<class IndividualType>
	void kTournament(std::mt19937& random, std::vector<IndividualType>& individuals, int number_to_select, int tournament_size)
	{
		for (size_t partition_index = 0; partition_index < (individuals.size() - number_to_select); ++partition_index)
		{
			int dead_index = random_int(random, partition_index, individuals.size() - 1);
			for (int i = 0; i < (tournament_size - 1); ++i)
			{
				int contestant = random_int(random, partition_index, individuals.size() - 1);
				// Selecting lowest scores, because reverse-tournament
				if (individuals[contestant].fitness < individuals[dead_index].fitness) dead_index = contestant;
			}
			std::swap(individuals[partition_index], individuals[dead_index]);
		}
		individuals.erase(individuals.begin(), individuals.begin() + (individuals.size() - number_to_select));
		std::sort(individuals.begin(), individuals.end(), [&](const IndividualType& a, const IndividualType& b) { return a.fitness > b.fitness; });
	}
}
