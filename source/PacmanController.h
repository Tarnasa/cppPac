/// The controller for Pacman

#pragma once

#include <functional>
#include <random>
#include <limits>

#include "GameState.h"
#include "Node.h"
#include "TreeGenerators.h"
#include "TreeOperators.h"
#include "Helpers.h"

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
				root.children.emplace_back(Brain::generateFullTree(random, max_levels));
			}
			else
			{
				root.children.emplace_back(Brain::generateTreeUpTo(random, max_levels));
			}
		}

		explicit PacmanController() {};

		PacmanController(std::mt19937& random) : root()
		{
			Brain::Node* sum_node = new Brain::SumNode();
			root.children.emplace_back(sum_node);
			sum_node->children.push_back(new Brain::ProductNode());
			sum_node->children[0]->children.push_back(new Brain::ConstantNode(-1.0));
			sum_node->children[0]->children.push_back(new Brain::PacmanToDotNode());
			sum_node->children.push_back(new Brain::ProductNode());
			sum_node->children[1]->children.push_back(new Brain::ConstantNode(100.0));
			sum_node->children[1]->children.push_back(new Brain::PacmanDotsEatenNode());
			// root.children.push_back(new Brain::ProductNode());
			// root.children[2]->children.push_back(new Brain::ConstantNode(0.01));
			// root.children[2]->children.push_back(new Brain::PacmanToGhostNode());
		};

		explicit PacmanController(const PacmanController& rhs)
		{
			root.children.emplace_back(Brain::createCopy(rhs.root.children[0]));
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
				new_state.MovePacman(deltas[i]);
				new_state.UpdateDistances();
				double fitness = Evaluate(new_state);
				if (fitness > best_fitness)
				{
					best_fitness = fitness;
					best_action = (PacmanAction)i;
				}
			}
			return best_action;
		}
	};
}
