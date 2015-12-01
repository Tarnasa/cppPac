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
	GhostController::GhostController ghost_controller;
	bool valid_fitness;
	std::unique_ptr<char> buffer;
	int game_fitness;
	int fitness;
	
	PacmanIndividual(std::mt19937& random, int max_levels, bool full) :
		pacman_controller(random, max_levels, full), ghost_controller(random), valid_fitness(false)
	{
		// Either full or grow
	}

	PacmanIndividual(std::mt19937& random) : pacman_controller(), ghost_controller(random), valid_fitness(false) {}

	PacmanIndividual(const PacmanIndividual& rhs) = delete;
	PacmanIndividual(PacmanIndividual&& rhs) = default;
	PacmanIndividual& operator=(PacmanIndividual&& rhs) = default;

	void evaluate(std::mt19937& random);

	void steal_buffer(PacmanIndividual& rhs);
};
