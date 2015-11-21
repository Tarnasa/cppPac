#pragma once

#include <vector>
#include <random>

#include "Helpers.h"
#include "Individual.h"

namespace ParentSelection
{
	std::vector<std::vector<int>> FPS(std::mt19937& random, const std::vector<Individual>& individuals, int number_to_select)
	{
		std::vector<std::vector<int>> parents;
		// Use stochastic acceptance instead of naive CDF (https://en.wikipedia.org/wiki/Fitness_proportionate_selection)
		int max_fitness = individuals[0].GetFitness(random);
		//int max_fitness = std::max_element(individuals.begin(), individuals.end(), [&](Individual& a) { return a.GetFitness(random); })->GetFitness(random);
		for (int child_index = 0; child_index < number_to_select; ++child_index)
		{
			std::vector<int> pair;
			while (pair.size() < 2)
			{
				int dart = random_int(random, 0, individuals.size() - 1);
				if (chance(random, (individuals[dart].GetFitness(random) + 1.0) / (max_fitness + 1.0))) // Add one so that even 0 fitness have a chance and we don't infinite loop
				{
					if (std::find(pair.cbegin(), pair.cend(), dart) == pair.cend())
					{
						pair.emplace_back(dart);
					}
				}
			}
			parents.emplace_back(pair);
		}
		return parents;
	}

	std::vector<std::vector<int>> overselection(std::mt19937& random, const std::vector<Individual>& individuals, int number_to_select)
	{
		std::vector<std::vector<int>> parents;
		// Find partition point
		double partition_percentage = 0.04;
		if (individuals.size() < 1000)
			partition_percentage = 0.32;
		else if (individuals.size() < 2000)
			partition_percentage = 0.16;
		else if (individuals.size() < 4000)
			partition_percentage = 0.08;
		int partition_index = partition_percentage * individuals.size();
		for (int c = 0; c < number_to_select; ++c)
		{
			std::vector<int> pair;
			while (pair.size() < 2)
			{
				int dart;
				// Pick 80% from top partition, 20% from bottom partition
				if (chance(random, 0.8))
					dart = random_int(random, 0, partition_index - 1);
				else
					dart = random_int(random, partition_index, individuals.size() - 1);
				// Add if not in pair
				if (std::find(pair.cbegin(), pair.cend(), dart) == pair.cend())
					pair.emplace_back(dart);
			}
			parents.emplace_back(pair);
		}
		return parents;
	}

	std::vector<Individual> generate_children(std::mt19937& random, const std::vector<Individual>& individuals, const std::vector<std::vector<int>>& parent_indices)
	{
		std::vector<Individual> children;
		for (auto&& pair : parent_indices)
		{
			children.emplace_back(random);
			children.emplace_back(random);
			Individual& left = children[children.size() - 2];
			Individual& right = children[children.size() - 1];
			left.pacman_controller.root.children.emplace_back(Brain::createCopy(individuals[pair[0]].pacman_controller.root.children[0]));
			right.pacman_controller.root.children.emplace_back(Brain::createCopy(individuals[pair[1]].pacman_controller.root.children[0]));
			Brain::crossover(random, &left.pacman_controller.root, &right.pacman_controller.root);
		}
		return children;
	}

	void truncate(std::vector<Individual>& individuals, int number_to_select)
	{
		individuals.erase(individuals.begin() + number_to_select, individuals.begin() + individuals.size());
	}

	void kTournament(std::mt19937& random, std::vector<Individual>& individuals, int number_to_select, int tournament_size)
	{
		for (int partition_index = 0; partition_index < (individuals.size() - number_to_select); ++partition_index)
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
		std::sort(individuals.begin(), individuals.end(), [&](const Individual& a, const Individual& b) { return a.fitness > b.fitness; });
	}
}
