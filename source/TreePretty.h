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

	std::string ToEquation(Node* node, char parent=0)
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
		case 'c': ans += std::to_string(dynamic_cast<ConstantNode*>(node)->constant); break;
		case 'D': ans += "pacman_to_dot"; break;
		case 'G': ans += "pacman_to_ghost"; break;
		case 'E': ans += "dots_eaten"; break;
		case 'X': ans += "pacman_x"; break;
		case 'Y': ans += "pacman_y"; break;
		}
		return ans;
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

	std::string Test2(std::mt19937& random)
	{
		BufferNode buffer;
		buffer.children.emplace_back(generate_tree_up_to(random, 5));
		return ToEquation(&buffer);
	}
}
