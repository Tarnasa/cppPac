// Functions which generate trees

#pragma once

#include <random>

#include "Node.h"

namespace Brain
{
	Node* generate_tree(std::mt19937& random, int levels, bool is_ghost);
	Node* generate_full_tree(std::mt19937& random, int levels, bool is_ghost);
	Node* generate_tree_up_to(std::mt19937& random, int max_levels, bool is_ghost);
	
	Node* generate_terminal_node(std::mt19937& random, bool is_ghost);
	Node* generate_parent_node(std::mt19937& random);
}
