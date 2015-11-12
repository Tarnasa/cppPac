/// Class representing a ghost

#pragma once

#include <string>

#include "Position.h"

class Ghost
{
public:
	int id;
	Position position;
	int distance_to_pacman;
	Ghost(int id, int x, int y) : id(id), position(x, y) {};
};