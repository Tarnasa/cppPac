#pragma once

#include <random>
#include <memory>

#include "PacmanController.h"
#include "GhostController.h"
#include "Game.h"

class PacmanIndividual
{
public:
	static int width, height, time_limit, buffer_size;
	static double density, parsimony_pressure;

	PacmanController::PacmanController pacman_controller;
	std::unique_ptr<char> buffer;
	int game_fitness;
	int fitness;
	
	PacmanIndividual(std::mt19937& random, int max_levels, bool full) :
		pacman_controller(random, max_levels, full)
	{
		// Either full or grow
	}

	PacmanIndividual() : pacman_controller() {}

	PacmanIndividual(const PacmanIndividual& rhs) = delete;
	PacmanIndividual(PacmanIndividual&& rhs) = default;
	PacmanIndividual& operator=(PacmanIndividual&& rhs) = default;

	void evaluate(std::mt19937& random, GhostController::GhostController& ghost_controller);

	void steal_buffer(PacmanIndividual& rhs);
};
