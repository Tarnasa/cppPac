#pragma once

#include <random>
#include <memory>

#include "GhostController.h"
#include "Game.h"
#include "TreeGetters.h"

class GhostIndividual
{
public:
	GhostController::GhostController ghost_controller;
	std::vector<std::shared_ptr<Game>> games;
	double fitness;
	double game_fitness;

	int penalty;

	GhostIndividual(std::mt19937& random, int max_levels, bool full, double parsimony_pressure) :
		ghost_controller(random, max_levels, full), fitness(0), game_fitness(0)
	{
		// Either full or grow
		penalty = static_cast<int>(parsimony_pressure * Brain::count_nodes(&ghost_controller.root));
	}

	double add_game(std::shared_ptr<Game> game);

	GhostIndividual() : ghost_controller(), games(), fitness(0), game_fitness(0), penalty(0) {}
	GhostIndividual(const GhostIndividual& rhs) = default;
	GhostIndividual(GhostIndividual&& rhs) = default;
	GhostIndividual& operator=(const GhostIndividual& rhs) = default;
	GhostIndividual& operator=(GhostIndividual&& rhs) = default;
};
