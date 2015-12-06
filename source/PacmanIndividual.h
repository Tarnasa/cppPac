#pragma once

#include <random>
#include <memory>

#include "PacmanController.h"
#include "Game.h"
#include "TreeGetters.h"

class PacmanIndividual
{
public:
	PacmanController::PacmanController pacman_controller;
	std::vector<std::shared_ptr<Game>> games;
	double fitness;
	double game_fitness;

	int penalty;
	
	PacmanIndividual(std::mt19937& random, int max_levels, bool full, double parsimony_pressure) :
		pacman_controller(random, max_levels, full), fitness(0), game_fitness(0)
	{
		// Either full or grow
		penalty = static_cast<int>(parsimony_pressure * Brain::count_nodes(&pacman_controller.root));
	}

	double add_game(std::shared_ptr<Game> game);

	PacmanIndividual() : pacman_controller(), games(), fitness(0), game_fitness(0), penalty(0) {}
	PacmanIndividual(const PacmanIndividual& rhs) = default;
	PacmanIndividual(PacmanIndividual&& rhs) = default;
	PacmanIndividual& operator=(const PacmanIndividual& rhs) = default;
	PacmanIndividual& operator=(PacmanIndividual&& rhs) = default;
};
