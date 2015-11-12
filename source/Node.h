/// Represents a node in GP tree

#pragma once

#include <random>
#include <functional>
#include <limits.h>

#include "Helpers.h"
#include "GameState.h"

namespace Brain
{
	// This converts the output floating point value to a decision by interpreting the valid range to be 0..1 and clamping all inputs to that range.
	template<typename T>
	T clamp_to_decisions(double input, T number_of_actions) // 1, 1, 1, 1, 2, 3, 3, 3, 3
	{
		return static_cast<T>(clamp(0, number_of_actions - 1, static_cast<int>(input * static_cast<int>(number_of_actions))));
	}

	// This converts the floating point signal into a decision by expanding the 0..1 range onto the decision domain, and then modulo-ing it back into the range
	template<typename T>
	T modulo_decisions(double input, T number_of_actions) // 1, 2, 3, 1, 2, 3, 1, 2, 3
	{
		return static_cast<T>((static_cast<int>(input * static_cast<int>(number_of_actions)) % number_of_actions + number_of_actions) % number_of_actions);
	}

	class Node
	{
	public:
		std::vector<Node*> children;
		virtual ~Node(); // deletes children
		virtual double Forward(const GameState& state) = 0; // This should call children's Forward()'s as well
		virtual int GetMinChildren() { return 0; }
		virtual int GetMaxChildren() { return INT_MAX; }
	};

	class RandomNode : public Node
	{
	public:
		std::function<double()> real_random;

		RandomNode(std::mt19937& random);
		virtual double Forward(const GameState& state);
		virtual int GetMaxChildren() { return 0; }
	};

	class RandomRangeNode : public Node
	{
	public:
		std::mt19937& random;

		RandomRangeNode(std::mt19937& random) : random(random) {};
		virtual double Forward(const GameState& state);
		virtual int GetMinChildren() { return 2; }
		virtual int GetMaxChildren() { return 2; }
	};

	class ConstantNode : public Node
	{
	public:
		const double constant;

		ConstantNode(double constant) : constant(constant) {};
		double Forward(const GameState& state);
		virtual int GetMaxChildren() { return 0; }
	};

	class SumNode : public Node
	{
	public:
		SumNode() {};

		double Forward(const GameState& state);
	};

	class ProductNode : public Node
	{
	public:
		ProductNode() {};

		double Forward(const GameState& state);
	};

	class PacmanToDot : public Node // Returns the distance from Pacman to the nearest dot
	{
	public:
		PacmanToDot() {};

		double Forward(const GameState& state);
		virtual int GetMaxChildren() { return 0; }
	};

	class PacmanToGhost : public Node // Returns the distance from Pacman to the nearest ghost
	{
	public:
		PacmanToGhost() {};

		double Forward(const GameState& state);
		virtual int GetMaxChildren() { return 0; }
	};

	class PacmanDotsEaten : public Node
	{
	public:
		PacmanDotsEaten() {};

		double Forward(const GameState& state) { return state.pacman.dots_eaten; }
		virtual int GetMaxChildren() { return 0; }
	};
}
