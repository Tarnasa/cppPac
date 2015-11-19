// Functions for converting to and from an equation representation of the controller tree

#pragma once

#include <string>
#include <regex>

#include "Node.h"

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
		case '|': ans += ToEquation(node->children[0]); break;
		case 'R': ans += "random_range(" + ToEquation(node->children[0]) + ", " + ToEquation(node->children[1]) + ")"; break;
		case '+': ans += Surround(ToEquation(node->children[0]) + " + " + ToEquation(node->children[1]), parent, "|R+-"); break;
		case '-': ans += Surround(ToEquation(node->children[0]) + " - " + ToEquation(node->children[1]), parent, "|R+-"); break;
		case '*': ans += Surround(ToEquation(node->children[0]) + " * " + ToEquation(node->children[1]), parent, "|R*"); break;
		case '/': ans += Surround(ToEquation(node->children[0]) + " / " + ToEquation(node->children[1]), parent, "|R"); break;
			// Terminals
		case 'r': ans += "random"; break;
		case 'c': ans += std::to_string(dynamic_cast<ConstantNode*>(node)->constant); break;
		case 'D': ans += "pacman_to_dot"; break;
		case 'G': ans += "pacman_to_ghost"; break;
		case 'E': ans += "dots_eaten"; break;
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
		return ToEquation(&buffer);
	}

	bool is_separator(char c)
	{
		//return (c == ' ' || c == '(' || c == ')' || c == ',');
		return c == ' ';
	}

	std::string head(std::string s)
	{
		static std::regex r("[a-zA-Z]|[(,)]");

		std::smatch match;
		std::regex_match(s, match, r);
		return std::string(match.cbegin(), match.cend());
	}

	std::string tail(std::string s)
	{
		static std::regex r("[a-zA-Z]|[(,)]");

		std::smatch match;
		std::regex_match(s, match, r);
		return s.substr(match.position + match.length);
	}

	Node* FromEquation(std::mt19937& random, const std::string& equation)
	{
		// Example input string: random_range(dots_eaten + 1.0, (pacman_to_dot - 2.0) * 3.0)
		std::string s = std::regex_replace(equation, std::regex("([(,)])"), " $1 "); // Pad parens and commas
		return Parse(random, s);
	}

	Node* Parse(std::mt19937& random, std::string& s) // This verion modifies s to strip out head
	{
		// random_range ( dots_eaten + 1.0 , ( pacman_to_dot - 2.0 ) * 3.0 )

		Node* node = nullptr;
		std::string h = head(s);

		if (h == "random_range(")
		{
			node = new RandomRangeNode(random);
			s = tail(s);
			node->children.emplace_back(Parse(random, s));
			node->children.emplace_back(Parse(random, s));
		}
		else if (h == "random")
		{
			node = new RandomNode(random);
		}
		else if (h == "pacman_to_dot")
		{
			node = new PacmanToDotNode();
		}
		else if (h == "pacman_to_ghost")
		{
			node = new PacmanToGhostNode();
		}
		else if (h == "dots_eaten")
		{
			node = new PacmanDotsEatenNode();
		}
		else if (h == "(")
		{

		}
		else // Infix operators
		{

		}

		return nullptr;
	}

}
