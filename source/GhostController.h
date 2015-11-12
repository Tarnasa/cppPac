/// The controller for a ghost

#pragma once

#include <functional>
#include <random>

#include "GameState.h"
#include "Helpers.h"
#include "Node.h"

namespace GhostController
{
	enum GhostAction {
		Right = 0, Down, Left, Up, GHOST_ACTION_COUNT
	};

	static const std::vector<Position> deltas = {
		Position(1, 0),
		Position(0, 1),
		Position(-1, 0),
		Position(0, -1)
	};

	class GhostController
	{
	public:
		Brain::RandomNode root;

		GhostController(std::mt19937& random) : root(random) {};

		double Evaluate(const GameState& state)
		{
			return root.Forward(state);
		}

		GhostAction Decide(const GameState& state)
		{
			double best_fitness = std::numeric_limits<double>::min();
			GhostAction best_action;
			for (int i = 0; i < GHOST_ACTION_COUNT; ++i)
			{
				GameState new_state(state);
				new_state.MoveGhost(i, deltas[i]);
				new_state.UpdateDistances();
				double fitness = Evaluate(new_state);
				if (fitness > best_fitness)
				{
					best_fitness = fitness;
					best_action = (GhostAction)i;
				}
			}
			return best_action;
		}
	};
}
