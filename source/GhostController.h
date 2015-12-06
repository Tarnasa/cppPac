/// The controller for a ghost

#pragma once

#include <random>

#include "GameState.h"
#include "Node.h"
#include "TreeOperators.h"
#include "TreeGenerators.h"

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
		Brain::BufferNode root;

		GhostController() {};

		GhostController(std::mt19937& random, int max_levels, bool full)
		{
			if (full)
			{
				root.children.emplace_back(Brain::generate_full_tree(random, max_levels, true));
			}
			else
			{
				root.children.emplace_back(Brain::generate_tree_up_to(random, max_levels, true));
			}
		}

		GhostController(const GhostController& rhs)
		{
			root.children.emplace_back(Brain::copy_tree(rhs.root.children[0]));
		}

		GhostController& operator=(const GhostController& rhs)
		{
			if (root.children.size() != 0) delete root.children[0];
			if (root.children.size() == 0) root.children.emplace_back(nullptr);
			root.children[0] = Brain::copy_tree(rhs.root.children[0]);
			return *this;
		}

		GhostController(GhostController&& rhs)
		{
			root = rhs.root;
			rhs.root.children.clear();
		}
		GhostController& operator=(GhostController&& rhs)
		{
			root = rhs.root;
			rhs.root.children.clear();
			return *this;
		}

		double Evaluate(const GameState& state)
		{
			return root.Forward(state);
		}

		GhostAction Decide(int id, const GameState& state)
		{
			double best_fitness = -std::numeric_limits<double>::max();
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
