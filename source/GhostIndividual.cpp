#include "GhostIndividual.h"

#include "TreeGetters.h"

// These globals will be set by main
int GhostIndividual::width = 0;
int GhostIndividual::height = 0;
double GhostIndividual::density = 0.0;
int GhostIndividual::time_limit = 0;
int GhostIndividual::buffer_size = 0;
double GhostIndividual::parsimony_pressure = 0.0;


void GhostIndividual::evaluate(std::mt19937& random, PacmanController::PacmanController& pacman_controller)
{
	buffer.reset(new char[buffer_size]);
	Game game(width, height, random);
	game.Initialize(random, density, time_limit, &pacman_controller, &ghost_controller);
	game_fitness = game.RunTillDone(buffer.get());
	int penalty = static_cast<int>(parsimony_pressure * Brain::count_nodes(&ghost_controller.root));
	fitness = game_fitness + 200 - penalty;
}

void GhostIndividual::evaluateFromGameFitness(int game_fitness)
{
	this->game_fitness = game_fitness;
	int penalty = static_cast<int>(parsimony_pressure * Brain::count_nodes(&ghost_controller.root));
	fitness = game_fitness + 200 - penalty;
}

void GhostIndividual::steal_buffer(GhostIndividual& rhs)
{
	ghost_controller = rhs.ghost_controller;
	buffer.reset(rhs.buffer.release());
	game_fitness = rhs.game_fitness;
	fitness = rhs.fitness;
}
