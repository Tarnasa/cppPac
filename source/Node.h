/// Represents a node in GP tree

#pragma once

#include <random>
#include <functional>
#include <limits.h>

#include "Helpers.h"
#include "GameState.h"

namespace Brain
{
	class Node
	{
	public:
		std::vector<Node*> children;
		virtual ~Node(); // deletes children
		virtual double Forward(const GameState& state) = 0; // This should call children's Forward()'s as well
		virtual Node* Clone() = 0;
		virtual char GetChar() = 0;
		virtual int GetMinChildren() { return 0; }
		virtual int GetMaxChildren() { return INT_MAX; }
	};

	enum NodeIds
	{
		Random = 0,
		RandomRange,
		Constant,
		Sum,
		Subtract,
		Product,
		Divide,
		PacmanToDot,
		PacmanToGhost,
		PacmanDotsEaten,
		NUMBER_OF_NODE_TYPES
	};

	Node* createNodeFromInt(std::mt19937& random, int index);

	class BufferNode : public Node // Special node for the root node in a tree
	{
	public:
		virtual double Forward(const GameState& state) { return children[0]->Forward(state); }
		virtual Node* Clone() { return new BufferNode(*this); }
		virtual char GetChar() { return '|'; }
		virtual int GetMinChildren() { return 1; }
		virtual int GetMaxChildren() { return 1; }
	};

	class RandomNode : public Node
	{
	public:
		std::function<double()> real_random;

		RandomNode(std::mt19937& random);
		virtual double Forward(const GameState& state);
		virtual Node* Clone() { return new RandomNode(*this); }
		virtual char GetChar() { return 'r'; }
		virtual int GetMaxChildren() { return 0; }
	};

	class RandomRangeNode : public Node
	{
	public:
		std::mt19937& random;

		RandomRangeNode(std::mt19937& random) : random(random) {};
		virtual double Forward(const GameState& state);
		virtual Node* Clone() { return new RandomRangeNode(*this); }
		virtual char GetChar() { return 'R'; }
		virtual int GetMinChildren() { return 2; }
		virtual int GetMaxChildren() { return 2; }
	};

	class ConstantNode : public Node
	{
	public:
		const double constant;

		ConstantNode(double constant) : constant(constant) {};
		double Forward(const GameState& state);
		virtual Node* Clone() { return new ConstantNode(*this); }
		virtual char GetChar() { return 'c'; }
		virtual int GetMaxChildren() { return 0; }
	};

	class SumNode : public Node
	{
	public:
		SumNode() {};

		double Forward(const GameState& state);
		virtual Node* Clone() { return new SumNode(*this); }
		virtual char GetChar() { return '+'; }
		virtual int GetMinChildren() { return 2; }
		virtual int GetMaxChildren() { return 2; }
	};

	class SubtractNode : public Node
	{
	public:
		SubtractNode() {};

		double Forward(const GameState& state);
		virtual Node* Clone() { return new SubtractNode(*this); }
		virtual char GetChar() { return '-'; }
		virtual int GetMinChildren() { return 2; }
		virtual int GetMaxChildren() { return 2; }
	};

	class ProductNode : public Node
	{
	public:
		ProductNode() {};

		double Forward(const GameState& state);
		virtual Node* Clone() { return new ProductNode(*this); }
		virtual char GetChar() { return '*'; }
		virtual int GetMinChildren() { return 2; }
		virtual int GetMaxChildren() { return 2; }
	};

	class DivideNode : public Node
	{
	public:
		DivideNode () {};

		double Forward(const GameState& state);
		virtual Node* Clone() { return new DivideNode(*this); }
		virtual char GetChar() { return '/'; }
		virtual int GetMinChildren() { return 2; }
		virtual int GetMaxChildren() { return 2; }
	};

	class PacmanToDotNode : public Node // Returns the distance from Pacman to the nearest dot
	{
	public:
		PacmanToDotNode() {};

		double Forward(const GameState& state) { return state.pacman.distance_to_dot; }
		virtual Node* Clone() { return new PacmanToDotNode(*this); }
		virtual char GetChar() { return 'D'; }
		virtual int GetMaxChildren() { return 0; }
	};

	class PacmanToGhostNode : public Node // Returns the distance from Pacman to the nearest ghost
	{
	public:
		PacmanToGhostNode() {};

		double Forward(const GameState& state) { return state.pacman.distance_to_ghost; }
		virtual Node* Clone() { return new PacmanToGhostNode(*this); }
		virtual char GetChar() { return 'G'; }
		virtual int GetMaxChildren() { return 0; }
	};

	class PacmanDotsEatenNode : public Node
	{
	public:
		PacmanDotsEatenNode() {};

		double Forward(const GameState& state) { return state.pacman.dots_eaten; }
		virtual Node* Clone() { return new PacmanDotsEatenNode(*this); }
		virtual char GetChar() { return 'E'; }
		virtual int GetMaxChildren() { return 0; }
	};
}
