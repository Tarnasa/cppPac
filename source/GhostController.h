/// The controller for a ghost

#pragma once

#include <random>

#include "GameState.h"
#include "Node.h"
#include <assert.h>

namespace GhostController
{
	enum GhostAction {
		Right = 0, Down, Left, Up, GHOST_ACTION_COUNT
	};

	static const std::vector<Position> deltas = {
		Position( 1,  0),
		Position( 0,  1),
		Position(-1,  0),
		Position( 0, -1)
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

		GhostAction Decide(int id, const GameState& state)
		{
			double best_fitness = std::numeric_limits<double>::min();
			GhostAction best_action = GHOST_ACTION_COUNT;
			for (int i = 0; i < GHOST_ACTION_COUNT; ++i)
			{
				GameState new_state(state);
				new_state.current_ghost = id; // Tell the sensor nodes which ghost to look at
				if (!new_state.MoveGhost(id, deltas[i])) continue;
				new_state.UpdateDistances();
				double fitness = Evaluate(new_state);
				if (fitness > best_fitness)
				{
					best_fitness = fitness;
					best_action = static_cast<GhostAction>(i);
				}
			}
			return best_action;
		}
	};
}
