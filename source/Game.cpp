/// An interactable Pacman game

#include <functional>
#include <algorithm>
#include <random>
#include <cstdio>
#include <limits>

#include "Game.h"
#include "Helpers.h"

Game::Game(int width, int height, std::mt19937& random) : state(width, height), game_over(false), pacman_controller(random)
{
	//
}

void Game::Initialize(std::mt19937& random, double density, int time_limit)
{
	this->initial_time_remaining = time_limit;
	this->time_remaining = time_limit;

	// Setup ghost controllers
	this->ghost_controllers.emplace_back(random);
	this->ghost_controllers.emplace_back(random);
	this->ghost_controllers.emplace_back(random);

	// Create random distributions
	auto real_random = [&]()
	{
		std::uniform_real_distribution<double> dist(0, 1);
		return dist(random);
	};

	auto int_random = [&]()
	{
		std::uniform_int_distribution<int> dist(1, state.dots.size() - 1);
		return dist(random);
	};

	// Fill the dots
	dots_placed = 0;
	for (size_t index = 1; index < state.dots.size(); ++index)
	{
		if (real_random() < density)
		{
			state.dots[index] = true;
			dots_placed += 1;
		}
	}
	// Place a dot if no dots were placed
	if (dots_placed == 0)
	{
		// Cannot be placed at the top left corner
		state.dots[int_random()] = true;
		dots_placed += 1;
	}
}

void Game::Step()
{
	state.UpdateDistances();

	// Have controllers decide their actions
	PacmanController::PacmanAction pacman_action = pacman_controller.Decide(state);
	std::vector<GhostController::GhostAction> ghost_actions;
	ghost_actions.reserve(ghost_controllers.size());
	for (size_t i = 0; i < ghost_controllers.size(); ++i)
	{
		ghost_actions.emplace_back(ghost_controllers[i].Decide(state));
	}

	// Move actors
	state.MovePacman(PacmanController::deltas[pacman_action]);
	for (size_t i = 0; i < ghost_controllers.size(); ++i)
	{
		state.MoveGhost(i, GhostController::deltas[ghost_actions[i]]);

		// Check if we intesected ms. pacman
		if (state.pacman.position == state.ghosts[i].position)
			game_over = true;
	}

	// Check for game over confitions
	time_remaining -= 1;
	if (time_remaining <= 0 || state.pacman.dots_eaten == dots_placed)
	{
		game_over = true;
	}
}

int Game::CalculateFitness()
{
	// Calculate fitness
	int percent_eaten = (state.pacman.dots_eaten * 100) / dots_placed;
	int percent_time_remaining = (time_remaining * 100) / initial_time_remaining;

	int fitness = (percent_eaten == 100) ? percent_eaten + percent_time_remaining : percent_eaten;
	return fitness;
}

int Game::RunTillDone(char* world_buffer)
{
	state.InitialLogEntry(world_buffer);
	world_buffer += sprintf(world_buffer, "t %i %i\n", time_remaining, state.pacman.dots_eaten);
	while (!game_over)
	{
		Step();

		state.StepLogEntry(world_buffer);
		world_buffer += sprintf(world_buffer, "t %i %i\n", time_remaining, state.pacman.dots_eaten);
	}

	return CalculateFitness();
}
