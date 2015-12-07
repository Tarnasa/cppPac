// Functions for converting to and from an equation representation of the controller tree

#pragma once

#include <string>
#include <regex>

#include "Node.h"
#include "TreeGenerators.h"
#include "TreeOperators.h"

namespace Brain
{
	std::string Surround(const std::string& s, char parent, std::string ignore_set)
	{
		for (char c : ignore_set)
		{
			if (c == parent) return s;
		}
		return "(" + s + ")";
	}

	std::string ToEquation(const Node* node, char parent=0)
	{
		std::string ans;
		switch (node->GetChar())
		{
			// Parents
		case '|': ans += ToEquation(node->children[0], '|'); break;
		case 'R': ans += "random_range(" + ToEquation(node->children[0], 'R') + ", " + ToEquation(node->children[1], 'R') + ")"; break;
		case '+': ans += Surround(ToEquation(node->children[0], '+') + " + " + ToEquation(node->children[1], '+'), parent, "|R+"); break;
		case '-': ans += Surround(ToEquation(node->children[0], '-') + " - " + ToEquation(node->children[1], '-'), parent, "|R"); break;
		case '*': ans += Surround(ToEquation(node->children[0], '*') + " * " + ToEquation(node->children[1], '*'), parent, "|R+-*"); break;
		case '/': ans += Surround(ToEquation(node->children[0], '/') + " / " + ToEquation(node->children[1], '/'), parent, "|R+-"); break;
			// Terminals
		case 'r': ans += "random"; break;
		case 'c': ans += std::to_string(dynamic_cast<ConstantNode*>(const_cast<Node*>(node))->constant); break;
		case 'D': ans += "pacman_to_dot"; break;
		case 'G': ans += "pacman_to_ghost"; break;
		case 'E': ans += "dots_eaten"; break;
		case 'X': ans += "pacman_x"; break;
		case 'Y': ans += "pacman_y"; break;
		case 'g': ans += "ghost_to_ghost"; break;
		case 'p': ans += "ghost_to_pacman"; break;
		}
		return ans;
	}

	std::string to_s_expression(const Node* node)
	{
		std::string ans;
		switch (node->GetChar())
		{
			// Parents
		case '|': ans += to_s_expression(node->children[0]); break;
		case 'R':
		case '+':
		case '-':
		case '*':
		case '/': ans += node->GetChar() + std::string("(") + to_s_expression(node->children[0]) + ", " + to_s_expression(node->children[1]) + ")"; break;
			// Terminals
		case 'c': ans += "c" + std::to_string(dynamic_cast<ConstantNode*>(const_cast<Node*>(node))->constant); break;
		case 'r':
		case 'D':
		case 'G':
		case 'E':
		case 'X':
		case 'Y':
		case 'g':
		case 'p': ans += node->GetChar(); break;
		}
		return ans;
	}

	Node* _from_s_expression(std::mt19937& random, const std::string& s, int& index)
	{
		if (index >= s.size())
		{
			return nullptr;
		}
		char head = s[index];
		Node* node = nullptr;
		size_t processed = -1;
		switch (head)
		{
			// Parents
		case '|': node = new Brain::BufferNode(); break;
		case 'R': node = new Brain::RandomRangeNode(random); break;
		case '+': node = new Brain::SumNode(); break;
		case '-': node = new Brain::SubtractNode(); break;
		case '*': node = new Brain::ProductNode(); break;
		case '/': node = new Brain::DivideNode(); break;
			// Terminals
		case 'c':
			node = new Brain::ConstantNode(std::stod(s.substr(index + 1), &processed)); break;
		case 'r': node = new Brain::RandomNode(random); break;
		case 'D': node = new Brain::PacmanToDotNode(); break;
		case 'G': node = new Brain::PacmanToGhostNode(); break;
		case 'E': node = new Brain::PacmanDotsEatenNode(); break;
		case 'X': node = new Brain::PacmanXNode(); break;
		case 'Y': node = new Brain::PacmanYNode(); break;
		case 'g': node = new Brain::GhostToGhostNode(); break;
		case 'p': node = new Brain::GhostToPacmanNode(); break;
		}
		// Buffer
		if (node->GetMaxChildren() == 1)
		{
			index += 1; // '|'
			node->children.emplace_back(_from_s_expression(random, s, index));
		}
		// Parents
		if (node->GetMaxChildren() == 2)
		{
			index += 2; // identifier and opening paren
			node->children.emplace_back(_from_s_expression(random, s, index));
			index += 2; // ", "
			node->children.emplace_back(_from_s_expression(random, s, index));
			index += 1; // closing paren
		}
		// Constant
		if (processed != -1)
		{
			index += processed + 1; // +1 because of leading 'c'
		}
		// Other terminals
		else if (node->GetMaxChildren() == 0)
		{
			index += 1;
		}
		return node;
	}

	Node* from_s_expression(std::mt19937& random, const std::string& s)
	{
		int index = 0;
		return _from_s_expression(random, s, index);
	}

	std::string Test(std::mt19937& random)
	{
		BufferNode buffer;
		buffer.children.emplace_back(new RandomRangeNode(random));
			buffer.children[0]->children.emplace_back(new SumNode());
				buffer.children[0]->children[0]->children.emplace_back(new ConstantNode(1.0));
				buffer.children[0]->children[0]->children.emplace_back(new PacmanDotsEatenNode());
			buffer.children[0]->children.emplace_back(new PacmanToDotNode());

		BufferNode buffer2;
		buffer2.children.emplace_back(new ProductNode());
			buffer2.children[0]->children.emplace_back(new RandomRangeNode(random));
				buffer2.children[0]->children[0]->children.emplace_back(new ConstantNode(2.0));
				buffer2.children[0]->children[0]->children.emplace_back(new ConstantNode(3.0));
			buffer2.children[0]->children.emplace_back(new ConstantNode(4.0));

		crossover(random, &buffer, &buffer2);
		return ToEquation(&buffer) + " AND " + ToEquation(&buffer2);
	}

	inline std::string test_sexp(std::mt19937& random)
	{
		BufferNode root;
		root.children.emplace_back(from_s_expression(random, "/(*(E, E), D)"));
		std::string big = ToEquation(&root);
		std::string small = to_s_expression(&root);
		return "Big: " + big + " , Small: " + small;
	}
}
