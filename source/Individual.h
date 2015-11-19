#pragma once

#include "PacmanController.h"
#include "GhostController.h"
#include "Game.h"

class Individual
{
public:
	static int width, height, density, time_limit, buffer_size;

	PacmanController::PacmanController pacman_controller;
	GhostController::GhostController ghost_controller;
	bool valid_fitness;
	std::unique_ptr<char> buffer;

private:
	int fitness;
	
public:
	Individual(std::mt19937& random, int max_levels, bool full) :
		pacman_controller(random, max_levels, full), ghost_controller(random), valid_fitness(false)
	{
		// Either full or grow
	};

	Individual(std::mt19937& random, PacmanController::PacmanController pacman_controller) :
		pacman_controller(random), ghost_controller(random), valid_fitness(false)
	{
		// Default non controllers
	};

	Individual(std::mt19937& random, const Individual& left_parent, const Individual& right_parent) :
		pacman_controller(random), ghost_controller(random), valid_fitness(false)
	{
		// Use crossover to generate controllers


	};

	void evaluate(std::mt19937& random)
	{
		if (!valid_fitness)
		{
			buffer.reset(new char[buffer_size]);
			Game game(width, height, random);
			game.Initialize(random, density, time_limit);
			int fitness = game.RunTillDone(buffer.get());
			valid_fitness = true;
		}
	}

	double GetFitness(std::mt19937& random)
	{
		if (!valid_fitness)
		{
			evaluate(random);
		}
		return fitness;
	}
};
