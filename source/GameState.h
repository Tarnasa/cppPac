/// This class represents the state of the game at any point in time

#pragma once

#include <vector>
#include <string>

#include "Position.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Rect.h"

class GameState
{
public:
	Rect bounds;
	std::vector<bool> dots;
	Pacman pacman;
	std::vector<Ghost> ghosts;

	int current_ghost; // For ghost controller sensor nodes

	GameState(int width, int height);

	static void InitializeNeighbors(int width, int height);

	bool GetDotAt(const Position& position) const;
	bool GetDotAt(int x, int y) const;
	void ResetDotAt(const Position& position);
	int GetIndex(const Position& position) const;
	int GetIndex(int x, int y) const;
	Position GetPosition(int index) const;
	bool IsGhostAt(const Position& position) const;

	bool MovePacman(Position delta);
	bool MoveGhost(int id, Position delta);

	void InitialLogEntry(char*& world_buffer) const;
	void StepLogEntry(char*& world_bufferg) const;

	void UpdateDistances();

	std::string ToString() const;

private:
	static std::vector<int> neighbors;

	int seen_counter;
	std::vector<int> seen;

	int GetSeenAt(const Position& position);
};
