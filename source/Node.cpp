#include "Node.h"

#include <limits>

namespace Brain
{
	Node::~Node()
	{
		for (auto&& child : children)
		{
			delete child;
		}
	};

	Node* createNodeFromInt(std::mt19937& random, int index)
	{
		switch (index)
		{
		case Constant: return new Brain::ConstantNode(std::uniform_real_distribution<double>()(random)); break;
		case Sum: return new Brain::SumNode(); break;
		case Subtract: return new Brain::SubtractNode(); break;
		case Product: return new Brain::ProductNode(); break;
		case Divide: return new Brain::DivideNode(); break;
		case Random: return new Brain::RandomNode(random); break;
		case RandomRange: return new Brain::RandomRangeNode(random); break;
		case PacmanToDot: return new Brain::PacmanToDotNode(); break;
		case PacmanToGhost: return new Brain::PacmanToGhostNode(); break;
		case PacmanDotsEaten: return new Brain::PacmanDotsEatenNode(); break;
		case PacmanX: return new Brain::PacmanXNode(); break;
		case PacmanY: return new Brain::PacmanYNode(); break;
		default: return nullptr; // lolwut
		}
	}

	RandomNode::RandomNode(std::mt19937& random) : real_random([&]() {
		std::uniform_real_distribution<double> dist(0, 1.0);
		return dist(random);
	}) {};
	double RandomNode::Forward(const GameState& state) { return real_random(); }

	double RandomRangeNode::Forward(const GameState& state)
	{
		double left = children[0]->Forward(state);
		double right = children[0]->Forward(state);
		double min = left;
		double max = right;
		if (left > right)
		{
			min = right;
			max = left;
		}
		// uniform_real can't handle special float cases
		if (min == -std::numeric_limits<double>::infinity()) return min;
		if (max == std::numeric_limits<double>::infinity()) return max;
		//if (left == std::numeric_limits<double>::quiet_NaN() || right == std::numeric_limits<double>::quiet_NaN()) return 0.0; // lol
		return std::uniform_real_distribution<double>(min, max)(random);
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

	double SubtractNode::Forward(const GameState& state)
	{
		return children[0]->Forward(state) - children[1]->Forward(state);
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

	double DivideNode::Forward(const GameState& state)
	{
		// Rely on:
		// https://en.wikipedia.org/wiki/IEEE_754-1985
		// To handle division by 0, and therefore return `infinity` (which might later be converted to `NaN`)
		
		// The following code is pointless because (max * 2) = infinity
		/*if (denominator == 0.0) // Yes, compare floating point for equallity
		{
			return std::numeric_limits<double>::max();
		}
		else if (denominator == -0.0) // Negative zero
		{
			return std::numeric_limits<double>::min();
		}*/
		double denominator = children[1]->Forward(state);
		return children[0]->Forward(state) / denominator;
	}
}
