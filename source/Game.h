/// An interactable Pacman game

#pragma once

#include <random>
#include <memory>
#include "PacmanController.h"
#include "GhostController.h"

// Forward declarations - because of kinda-circular dependency
class PacmanIndividual;
class GhostIndividual;

// This design pattern probably breaks so many rules
class Game
{
public:
	static int width, height, time_limit, buffer_size;
	static double density;

	int dots_placed;
	int fitness;
	std::unique_ptr<char> buffer;

	// Bluh
	PacmanController::PacmanController pacman_controller;
	GhostController::GhostController ghost_controller;
	// End Bluh

	static void fight(std::mt19937& random, PacmanIndividual& pacman, GhostIndividual& ghost);

//private:
	Game(std::mt19937& random, PacmanIndividual& pacman, GhostIndividual& ghost); // Should only be constructed from within fight()
};
