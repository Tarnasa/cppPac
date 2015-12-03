#include "PacmanIndividual.h"

#include "TreeGetters.h"

// These globals will be set by main
int PacmanIndividual::width = 0;
int PacmanIndividual::height = 0;
double PacmanIndividual::density = 0.0;
int PacmanIndividual::time_limit = 0;
int PacmanIndividual::buffer_size = 0;
double PacmanIndividual::parsimony_pressure = 0.0;


void PacmanIndividual::evaluate(std::mt19937& random)
{
	if (!valid_fitness)
	{
		buffer.reset(new char[buffer_size]);
		Game game(width, height, random);
		game.Initialize(random, density, time_limit, &pacman_controller, &ghost_controller);
		game_fitness = game.RunTillDone(buffer.get());
		int penalty = static_cast<int>(parsimony_pressure * Brain::count_nodes(&pacman_controller.root));
		fitness = game_fitness + 200 - penalty;
		valid_fitness = true;
	}
}

void PacmanIndividual::steal_buffer(PacmanIndividual& rhs)
{
	pacman_controller = rhs.pacman_controller;
	// TODO: Ghost controller
	buffer.reset(rhs.buffer.release());
	game_fitness = rhs.game_fitness;
	fitness = rhs.fitness;
	valid_fitness = rhs.valid_fitness;
}
