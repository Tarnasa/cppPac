/// The controller for Pacman

#pragma once

#include <random>

#include "GameState.h"
#include "Node.h"
#include "TreeGenerators.h"
#include "TreeOperators.h"

namespace PacmanController
{
	enum PacmanAction {
		Right = 0, Down, Left, Up, Hold, PACMAN_ACTION_COUNT
	};

	static const std::vector<Position> deltas = {
		Position(1, 0),
		Position(0, 1),
		Position(-1, 0),
		Position(0, -1),
		Position(0, 0)
	};

	class PacmanController
	{
	public:
		Brain::BufferNode root;

		PacmanController(std::mt19937& random, int max_levels, bool full)
		{
			if (full)
			{
				root.children.emplace_back(Brain::generate_full_tree(random, max_levels, false));
			}
			else
			{
				root.children.emplace_back(Brain::generate_tree_up_to(random, max_levels, false));
			}
		}

		explicit PacmanController() {};

		explicit PacmanController(const PacmanController& rhs)
		{
			root.children.emplace_back(Brain::copy_tree(rhs.root.children[0]));
		}

		PacmanController& operator=(const PacmanController& rhs)
		{
			if (root.children.size() != 0) delete root.children[0];
			if (root.children.size() == 0) root.children.emplace_back(nullptr);
			root.children[0] = Brain::copy_tree(rhs.root.children[0]);
			return *this;
		}

		PacmanController(PacmanController&& rhs)
		{
			root = rhs.root;
			rhs.root.children.clear();
		}
		PacmanController& operator=(PacmanController&& rhs)
		{
			root = rhs.root;
			rhs.root.children.clear();
			return *this;
		}

		double Evaluate(const GameState& state)
		{
			return root.Forward(state);
		}
	
		PacmanAction Decide(const GameState& state)
		{
			double best_fitness = Evaluate(state);
			PacmanAction best_action = Hold;
			for (int i = 0; i < Hold; ++i)
			{
				GameState new_state(state);
				if (!new_state.MovePacman(deltas[i])) continue;
				new_state.UpdateDistances();
				double fitness = Evaluate(new_state);
				if (fitness > best_fitness)
				{
					best_fitness = fitness;
					best_action = static_cast<PacmanAction>(i);
				}
			}
			return best_action;
		}
	};
}
