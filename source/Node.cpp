#include "Node.h"

namespace Brain
{
	Node::~Node()
	{
		for (auto&& child : children)
		{
			delete child;
		}
	};

	RandomNode::RandomNode(std::mt19937& random) : real_random([&]() {
		std::uniform_real_distribution<double> dist(0, 1.0);
		return dist(random);
	}) {};
	double RandomNode::Forward(const GameState& state)
	{
		return real_random();
	}

	double RandomRangeNode::Forward(const GameState& state)
	{
		return std::uniform_real_distribution<double>(children[0]->Forward(state), children[1]->Forward(state))(random);
	}

	double ConstantNode::Forward(const GameState& state)
	{
		return constant;
	}

	double SumNode::Forward(const GameState& state)
	{
		double sum = 0;
		for (auto&& child : children)
		{
			sum += child->Forward(state);
		}
		return sum;
	}

	double ProductNode::Forward(const GameState& state)
	{
		double product = 1;
		for (auto&& child : children)
		{
			product *= child->Forward(state);
		}
		return product;
	}

	double PacmanToDot::Forward(const GameState& state)
	{
		return state.pacman.distance_to_dot;
	}

	double PacmanToGhost::Forward(const GameState& state)
	{
		return state.pacman.distance_to_ghost;
	}
}