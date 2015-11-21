#pragma once

#include <random>

#include "PacmanController.h"
#include "GhostController.h"
#include "Game.h"

class Individual
{
public:
	static int width, height, time_limit, buffer_size;
	static double density;

	PacmanController::PacmanController pacman_controller;
	GhostController::GhostController ghost_controller;
	bool valid_fitness;
	std::unique_ptr<char> buffer;
	int fitness;
	
public:
	Individual(std::mt19937& random, int max_levels, bool full) :
		pacman_controller(random, max_levels, full), ghost_controller(random), valid_fitness(false)
	{
		// Either full or grow
	}

	Individual(std::mt19937& random) : pacman_controller(), ghost_controller(random), valid_fitness(false) {}

	Individual(const Individual& rhs) = delete;
	Individual(Individual&& rhs) = default;
	Individual& operator=(Individual&& rhs) = default;

	void evaluate(std::mt19937& random);

	double GetFitness(std::mt19937& random) const;

	void StealBuffer(Individual& rhs);
};
