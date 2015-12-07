/// An interactable Pacman game

#pragma once

#include <random>
#include <memory>

// Forward declarations
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



	static void fight(std::mt19937& random, PacmanIndividual& pacman, GhostIndividual& ghost);

//private:
	Game(std::mt19937& random, PacmanIndividual& pacman, GhostIndividual& ghost); // Should only be constructed from within fight()
};
