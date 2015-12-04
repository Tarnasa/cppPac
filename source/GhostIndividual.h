#pragma once

#include <random>
#include <memory>

#include "PacmanController.h"
#include "GhostController.h"
#include "Game.h"

class GhostIndividual
{
public:
	static int width, height, time_limit, buffer_size;
	static double density, parsimony_pressure;

	GhostController::GhostController ghost_controller;
	std::unique_ptr<char> buffer;
	int game_fitness;
	int fitness;

	GhostIndividual(std::mt19937& random, int max_levels, bool full) :
		ghost_controller(random, max_levels, full)
	{
		// Either full or grow
	}

	GhostIndividual() : ghost_controller() {}

	GhostIndividual(const GhostIndividual& rhs) = delete;
	GhostIndividual(GhostIndividual&& rhs) = default;
	GhostIndividual& operator=(GhostIndividual&& rhs) = default;

	void evaluate(std::mt19937& random, PacmanController::PacmanController& pacman_controller);
	void evaluateFromGameFitness(int game_fitness);

	void steal_buffer(GhostIndividual& rhs);
};
