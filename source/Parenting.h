#pragma once

#include <vector>
#include <random>

#include "PacmanIndividual.h"
#include "GhostIndividual.h"

namespace Parenting
{
	template<class IndividualType>
	std::vector<std::vector<int>> FPS(std::mt19937& random, const std::vector<IndividualType>& individuals, int number_to_select)
	{
		std::vector<std::vector<int>> parents;
		// Use stochastic acceptance instead of CDF (https://en.wikipedia.org/wiki/Fitness_proportionate_selection)
		int max_fitness = individuals[0].fitness;
		//int max_fitness = std::max_element(individuals.begin(), individuals.end(), [&](Individual& a) { return a.GetFitness(random); })->GetFitness(random);
		for (int child_index = 0; child_index < number_to_select; ++child_index)
		{
			std::vector<int> pair;
			while (pair.size() < 2)
			{
				int dart = random_int(random, 0, individuals.size() - 1);
				if (chance(random, (individuals[dart].fitness + 1.0) / (max_fitness + 1.0))) // Add one so that even 0 fitness have a chance and we don't infinite loop
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

	template<class PacmanIndividual>
	std::vector<std::vector<int>> overselection(std::mt19937& random, const std::vector<PacmanIndividual>& individuals, int number_to_select)
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
		int partition_index = static_cast<int>(partition_percentage * individuals.size());
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

	std::vector<PacmanIndividual> generate_children(std::mt19937& random, const std::vector<PacmanIndividual>& individuals, const std::vector<std::vector<int>>& parent_indices)
	{
		std::vector<PacmanIndividual> children;
		for (auto&& pair : parent_indices)
		{
			children.emplace_back();
			children.emplace_back();
			PacmanIndividual& left = children[children.size() - 2];
			PacmanIndividual& right = children[children.size() - 1];
			left.pacman_controller.root.children.emplace_back(Brain::copy_tree(individuals[pair[0]].pacman_controller.root.children[0]));
			right.pacman_controller.root.children.emplace_back(Brain::copy_tree(individuals[pair[1]].pacman_controller.root.children[0]));
			Brain::crossover(random, &left.pacman_controller.root, &right.pacman_controller.root);
		}
		return children;
	}

	std::vector<GhostIndividual> generate_children(std::mt19937& random, const std::vector<GhostIndividual>& individuals, const std::vector<std::vector<int>>& parent_indices)
	{
		std::vector<GhostIndividual> children;
		for (auto&& pair : parent_indices)
		{
			children.emplace_back();
			children.emplace_back();
			GhostIndividual& left = children[children.size() - 2];
			GhostIndividual& right = children[children.size() - 1];
			left.ghost_controller.root.children.emplace_back(Brain::copy_tree(individuals[pair[0]].ghost_controller.root.children[0]));
			right.ghost_controller.root.children.emplace_back(Brain::copy_tree(individuals[pair[1]].ghost_controller.root.children[0]));
			Brain::crossover(random, &left.ghost_controller.root, &right.ghost_controller.root);
		}
		return children;
	}
}
