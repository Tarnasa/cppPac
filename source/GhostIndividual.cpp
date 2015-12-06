#include "GhostIndividual.h"

double GhostIndividual::add_game(std::shared_ptr<Game> game)
{
	games.emplace_back(game);
	int adjusted_fitness = 400 - game->fitness - penalty;
	fitness = ((games.size() - 1) * fitness + adjusted_fitness) / games.size();
	game_fitness = ((games.size() - 1) * game_fitness + game->fitness) / games.size();
	return fitness;
}
