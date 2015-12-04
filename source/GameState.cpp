#include <algorithm>
#include <sstream>
#include <cstdio>
#include <limits.h>

#include "Helpers.h"
#include "GameState.h"

GameState::GameState(int width, int height) :
	bounds(0, 0, width - 1, height - 1),
	dots(width * height, false),
	pacman(0, 0),
	ghosts(),
	current_ghost(0),
	seen_counter(INT_MIN + 1),
	seen(width * height, INT_MIN)
{
	// All ghosts start in the lower right hand corner
	ghosts.emplace_back(0, width - 1, height - 1);
	ghosts.emplace_back(1, width - 1, height - 1);
	ghosts.emplace_back(2, width - 1, height - 1);
}

void GameState::InitializeNeighbors(int width, int height)
{
	static int x_off[] = { 1, 0, -1, 0 };
	static int y_off[] = { 0, 1, 0, -1 };

	Rect bounds(0, 0, width - 1, height - 1);

	neighbors = std::vector<int>(width * height * 4); // (right, down, left, up), ...
	Position p(0, 0);
	for (p.y = 0; p.y < height; ++p.y)
	{
		for (p.x = 0; p.x < width; ++p.x)
		{
			int index = (p.x + p.y * width) * 4;
			for (int i = 0; i < 4; ++i)
			{
				Position n = p + Position(x_off[i], y_off[i]);
				neighbors[index++] = bounds.Inside(n) ? (n.x + n.y * width) : p.x + p.y * width;
			}
		}
	}
}

bool GameState::GetDotAt(const Position& position) const
{
	return GetDotAt(position.x, position.y);
}

bool GameState::GetDotAt(int x, int y) const
{
	return bounds.Inside(x, y) ? dots[GetIndex(x, y)] : false;
}

void GameState::ResetDotAt(const Position& position)
{
	if (bounds.Inside(position))
		dots[GetIndex(position)] = false;
}

int GameState::GetIndex(const Position& position) const
{
	return GetIndex(position.x, position.y);
}

int GameState::GetIndex(int x, int y) const
{
	return x + y * (bounds.right + 1);
}

Position GameState::GetPosition(int index) const
{
	return Position(index % (bounds.right + 1), index / (bounds.right + 1));
}

bool GameState::IsGhostAt(const Position& position) const
{
	return any(ghosts, [&position](const Ghost& ghost)->bool {return ghost.position == position; });
}

bool GameState::MovePacman(Position delta)
{
#if !defined(NDEBUG)
	if (abs(delta) > 1)
		return false;
#endif
	if (!bounds.Inside(pacman.position + delta))
		return false;
	if (IsGhostAt(pacman.position + delta))
		return false;
	// Move ms. pacman
	pacman.position += delta;
	// Eat dots
	if (GetDotAt(pacman.position))
	{
		pacman.dots_eaten += 1;
		ResetDotAt(pacman.position);
	}
	return true;
}

bool GameState::MoveGhost(int id, Position delta)
{
#if !defined(NDEBUG)
	if (abs(delta) != 1)
		return false;
	if (id < 0 || id >= (int)ghosts.size())
		return false;
#endif
	if (!bounds.Inside(ghosts[id].position + delta))
		return false;
	// Move ghost
	ghosts[id].position += delta;

	return true;
}

void GameState::InitialLogEntry(char*& world_buffer) const
{
	world_buffer += sprintf(world_buffer, "%i\n%i\n", bounds.right + 1, bounds.bottom + 1);
	StepLogEntry(world_buffer);
	// Write the positions of all the dots
	int index = 0;
	for (int y = 0; y < bounds.GetWidth(); ++y)
	{
		for (int x = 0; x < bounds.GetHeight(); ++x)
		{
			if (dots[index])
			{
				world_buffer += sprintf(world_buffer, "p %i %i\n", x, y);
			}
			index += 1;
		}
	}
}

void GameState::StepLogEntry(char*& world_buffer) const
{
	// Write pacman and ghosts locations
	world_buffer += sprintf(world_buffer, "m %i %i\n", pacman.position.x, pacman.position.y);
	for (size_t ghost_index = 0; ghost_index < ghosts.size(); ++ghost_index)
	{
		world_buffer += sprintf(world_buffer, "%i %i %i\n", static_cast<int>(ghost_index + 1), ghosts[ghost_index].position.x, ghosts[ghost_index].position.y);
	}
}

void GameState::UpdateDistances()
{
	// GOTTA GO FAST
	int smaller_dimension = bounds.GetWidth() < bounds.GetHeight() ? bounds.GetWidth() : bounds.GetHeight();
	int buffer_size = smaller_dimension * 2;

	// Circular array/queue thing
	int* buffer = new int[buffer_size]; // TODO: Move this to static
	int first = 0;
	int next = 0;
	int size = 0;

	buffer[next++] = GetIndex(pacman.position); ++size; next %= buffer_size;
	seen[buffer[first]] = seen_counter;
	while (size)
	{
		int top = buffer[first++]; --size; (first += buffer_size) %= buffer_size;
		if (dots[top])
		{
			pacman.distance_to_dot = abs(GetPosition(top) - pacman.position);
			break;
		}
		for (int i = top*4; i < top*4 + 4; ++i)
		{
			int neighbor = neighbors[i];
			if (seen[neighbor] < seen_counter)
			{
				seen[neighbor] = seen_counter;
				buffer[next++] = neighbor; ++size; next %= buffer_size;
			}
		}
	}

	delete[] buffer;
	seen_counter += 1;

	// Calculate distance from pacman to ghosts
	pacman.distance_to_ghost = std::numeric_limits<int>::max();
	for (auto&& ghost : ghosts)
	{
		ghost.distance_to_pacman = abs(ghost.position - pacman.position);
		if (ghost.distance_to_pacman < pacman.distance_to_ghost)
		{
			pacman.distance_to_ghost = ghost.distance_to_pacman;
		}
	}

	// Find distance between current_ghost and nearest ghost
	ghosts[current_ghost].distance_to_ghost = std::numeric_limits<int>::max();
	for (auto&& other_ghost : ghosts)
	{
		if (other_ghost.id != current_ghost)
		{
			int distance = abs(other_ghost.position - ghosts[current_ghost].position);
			ghosts[current_ghost].distance_to_ghost = distance < ghosts[current_ghost].distance_to_ghost ? distance : ghosts[current_ghost].distance_to_ghost;
		}
	}
}

std::string GameState::ToString() const
{
	// Helper function for when you just need to SEE STUFF
	std::string s;
	int index = 0;
	for (int y = 0; y <= bounds.bottom; ++y)
	{
		for (int x = 0; x <= bounds.right; ++x)
		{
			char c = ' ';
			if (dots[index])
				c = '.';
			if (IsGhostAt(Position(x, y)))
				c = 'G';
			if (index == GetIndex(pacman.position))
				c = 'P';
			s += c;
			index += 1;
		}
		s += '\n';
	}
	return s;
}

std::vector<int> GameState::neighbors;

int GameState::GetSeenAt(const Position& position)
{
	return bounds.Inside(position) ? seen[GetIndex(position)] : std::numeric_limits<int>::max();
}
