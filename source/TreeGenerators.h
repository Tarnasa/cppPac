// Functions which generate trees

#pragma once

#include <random>

#include "Node.h"

namespace Brain
{
	Node* generateTree(std::mt19937& random, int levels);
	Node* generateFullTree(std::mt19937& random, int levels);
	Node* generateTreeUpTo(std::mt19937& random, int max_levels);
	
	Node* generateTerminalNode(std::mt19937& random);
	Node* generateParentNode(std::mt19937& random);
}
