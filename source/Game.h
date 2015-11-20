/// An interactable Pacman game

#pragma once

#include <string>
#include <functional>
#include <random>
#include <fstream>

#include "GameState.h"
#include "PacmanController.h"
#include "GhostController.h"

class Game
{
public:
	GameState state;
	bool game_over;
	int initial_time_remaining;
	int time_remaining;
	int dots_placed;
	PacmanController::PacmanController* pacman_controller;
	GhostController::GhostController* ghost_controller;

	Game(int width, int height, std::mt19937& random);
	
	void Initialize(std::mt19937& random, double density, int time_remaining,
		PacmanController::PacmanController* pacman_controller,
		GhostController::GhostController* ghost_controller);

	void Step();
	int RunTillDone(char* world_buffer);

	int CalculateFitness();
};
