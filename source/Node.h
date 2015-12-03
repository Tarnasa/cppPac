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
		virtual Node* Clone() const = 0;
		virtual char GetChar() const = 0;
		virtual int GetMinChildren() const { return 0; }
		virtual int GetMaxChildren() const { return INT_MAX; }
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
		PacmanX,
		PacmanY,
		GhostToGhost,
		GhostToPacman,
		NUMBER_OF_NODE_TYPES
	};

	Node* createNodeFromInt(std::mt19937& random, int index);

	class TerminalNode : public Node
	{
	public:
		virtual int GetMinChildren() const { return 0; }
		virtual int GetMaxChildren() const { return 0; }
	};

	class BinaryNode : public Node
	{
	public:
		virtual int GetMinChildren() const { return 2; }
		virtual int GetMaxChildren() const { return 2; }
	};

	class BufferNode : public Node // Special node for the root node in a tree
	{
	public:
		virtual double Forward(const GameState& state) { return children[0]->Forward(state); }
		virtual Node* Clone() const { return new BufferNode(*this); }
		virtual char GetChar() const { return '|'; }
		virtual int GetMinChildren() const { return 1; }
		virtual int GetMaxChildren() const { return 1; }
	};

	class RandomNode : public TerminalNode
	{
	public:
		std::function<double()> real_random;

		RandomNode(std::mt19937& random);
		virtual double Forward(const GameState& state);
		virtual Node* Clone() const { return new RandomNode(*this); }
		virtual char GetChar() const { return 'r'; }
	};

	class RandomRangeNode : public BinaryNode
	{
	public:
		std::mt19937& random;

		RandomRangeNode(std::mt19937& random) : random(random) {};
		virtual double Forward(const GameState& state);
		virtual Node* Clone() const { return new RandomRangeNode(*this); }
		virtual char GetChar() const { return 'R'; }
	};

	class ConstantNode : public TerminalNode
	{
	public:
		const double constant;

		ConstantNode(double constant) : constant(constant) {};
		double Forward(const GameState& state);
		virtual Node* Clone() const { return new ConstantNode(*this); }
		virtual char GetChar() const { return 'c'; }
	};

	class SumNode : public BinaryNode
	{
	public:
		SumNode() {};

		double Forward(const GameState& state);
		virtual Node* Clone() const { return new SumNode(*this); }
		virtual char GetChar() const { return '+'; }
	};

	class SubtractNode : public BinaryNode
	{
	public:
		SubtractNode() {};

		double Forward(const GameState& state);
		virtual Node* Clone() const { return new SubtractNode(*this); }
		virtual char GetChar() const { return '-'; }
	};

	class ProductNode : public BinaryNode
	{
	public:
		ProductNode() {};

		double Forward(const GameState& state);
		virtual Node* Clone() const { return new ProductNode(*this); }
		virtual char GetChar() const { return '*'; }
	};

	class DivideNode : public BinaryNode
	{
	public:
		DivideNode () {};

		double Forward(const GameState& state);
		virtual Node* Clone() const { return new DivideNode(*this); }
		virtual char GetChar() const { return '/'; }
	};

	class PacmanToDotNode : public TerminalNode
	{
	public:
		PacmanToDotNode() {};

		double Forward(const GameState& state) { return state.pacman.distance_to_dot; }
		virtual Node* Clone() const { return new PacmanToDotNode(*this); }
		virtual char GetChar() const { return 'D'; }
	};

	class PacmanToGhostNode : public TerminalNode
	{
	public:
		PacmanToGhostNode() {};

		double Forward(const GameState& state) { return state.pacman.distance_to_ghost; }
		virtual Node* Clone() const { return new PacmanToGhostNode(*this); }
		virtual char GetChar() const { return 'G'; }
	};

	class PacmanDotsEatenNode : public TerminalNode
	{
	public:
		PacmanDotsEatenNode() {};

		double Forward(const GameState& state) { return state.pacman.dots_eaten; }
		virtual Node* Clone() const { return new PacmanDotsEatenNode(*this); }
		virtual char GetChar() const { return 'E'; }
	};

	class PacmanXNode : public TerminalNode
	{
	public:
		PacmanXNode() {};

		double Forward(const GameState& state) { return state.pacman.position.x; }
		virtual Node* Clone() const { return new PacmanXNode(*this); }
		virtual char GetChar() const { return 'X'; }
	};
	
	class PacmanYNode : public TerminalNode
	{
	public:
		PacmanYNode() {};

		double Forward(const GameState& state) { return state.pacman.position.y; }
		virtual Node* Clone() const { return new PacmanYNode(*this); }
		virtual char GetChar() const { return 'Y'; }
	};

	class GhostToGhostNode : public TerminalNode
	{
	public:
		GhostToGhostNode() {};

		double Forward(const GameState& state) { return state.ghosts[state.current_ghost].distance_to_ghost; }
		virtual Node* Clone() const { return new GhostToGhostNode(*this); }
		virtual char GetChar() const { return 'g'; }
	};

	class GhostToPacmanNode : public TerminalNode
	{
	public:
		GhostToPacmanNode() {};

		double Forward(const GameState& state) { return state.ghosts[state.current_ghost].distance_to_pacman; }
		virtual Node* Clone() const { return new GhostToPacmanNode(*this); }
		virtual char GetChar() const { return 'p'; }
	};
}
