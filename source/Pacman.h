/// Class representing Ms. Pacman

#pragma once

#include "Position.h"

class Pacman
{
public:
	Position position;
	int dots_eaten;
	int distance_to_dot;
	int distance_to_Wall;
	int distance_to_ghost;
	Pacman(int x, int y) : position(x, y), dots_eaten(0) {};
};
