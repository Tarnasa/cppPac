#include "Individual.h"

int Individual::width = 0;
int Individual::height = 0;
double Individual::density = 0;
int Individual::time_limit = 0;
int Individual::buffer_size = 0;

void Individual::evaluate(std::mt19937& random)
{
	if (!valid_fitness)
	{
		buffer.reset(new char[buffer_size]);
		Game game(width, height, random);
		game.Initialize(random, density, time_limit, &pacman_controller, &ghost_controller);
		fitness = game.RunTillDone(buffer.get());
		valid_fitness = true;
	}
}

double Individual::GetFitness(std::mt19937& random) const
{
	return fitness;
}
