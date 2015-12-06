/// An entire fight between a PacmanIndividual and GhostIndividual

#include <random>
#include <cstdio>
#include <memory>

#include "Game.h"
#include "Helpers.h"

#include "PacmanIndividual.h"
#include "GhostIndividual.h"

int Game::width, Game::height, Game::time_limit, Game::buffer_size;
double Game::density;

void Game::fight(std::mt19937& random, PacmanIndividual& pacman, GhostIndividual& ghost)
{
	auto game = std::make_shared<Game>(random, pacman, ghost);

	pacman.add_game(game);
	ghost.add_game(game);
}

Game::Game(std::mt19937& random, PacmanIndividual& pacman, GhostIndividual& ghost) :
	buffer(new char[buffer_size])
{
	int time_remaining = time_limit;

	GameState state(width, height);

	// Fill the dots
	dots_placed = 0;
	for (size_t index = 1; index < state.dots.size(); ++index)
	{
		if (chance(random, density))
		{
			state.dots[index] = true;
			dots_placed += 1;
		}
	}
	// Place a dot if no dots were placed
	if (dots_placed == 0)
	{
		// Cannot be placed at the top left corner
		state.dots[random_int(random, 1, state.dots.size() - 1)] = true;
		dots_placed += 1;
	}

	state.UpdateDistances();

	char* running_pointer = buffer.get();

	state.InitialLogEntry(running_pointer);
	running_pointer += sprintf(running_pointer, "t %i %i\n", time_remaining, state.pacman.dots_eaten);
	bool game_over = false;
	while (!game_over)
	{
		// Have controllers decide their actions
		PacmanController::PacmanAction pacman_action = pacman.pacman_controller.Decide(state);
		std::vector<GhostController::GhostAction> ghost_actions;
		ghost_actions.reserve(state.ghosts.size());
		for (size_t i = 0; i < state.ghosts.size(); ++i)
		{
			ghost_actions.emplace_back(ghost.ghost_controller.Decide(i, state));
		}

		// Move actors
		state.MovePacman(PacmanController::deltas[pacman_action]);
		for (size_t i = 0; i < state.ghosts.size(); ++i)
		{
			state.MoveGhost(i, GhostController::deltas[ghost_actions[i]]);

			// Check if ghost intersected Ms. Pacman
			if (state.pacman.position == state.ghosts[i].position)
				game_over = true;
		}

		// Check for game over conditions
		time_remaining -= 1;
		if (time_remaining <= 0 || state.pacman.dots_eaten == dots_placed)
		{
			game_over = true;
		}

		state.StepLogEntry(running_pointer);
		running_pointer += sprintf(running_pointer, "t %i %i\n", time_remaining, state.pacman.dots_eaten);
	}

	// Calculate fitness
	int percent_eaten = (state.pacman.dots_eaten * 100) / dots_placed;
	int percent_time_remaining = (time_remaining * 100) / time_limit;

	fitness = (percent_eaten == 100) ? percent_eaten + percent_time_remaining : percent_eaten;
}
