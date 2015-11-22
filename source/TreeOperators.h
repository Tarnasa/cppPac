// Genetic operators for mutating / combining trees

#pragma once

#include "Node.h"

namespace Brain
{
	Node* copy_tree(const Node* root);

	void replace_one_node(std::mt19937& random, Node*& node); // Replaces one random node in the subtree
	void replace(std::mt19937& random, Node*& node); // Replaces node with a new tree of equal height
	void grow(std::mt19937& random, Node*& node, int max_levels); // Replaces a leaf node with a new tree of possibly greater height

	void mutate(std::mt19937& random, BufferNode* node); // Randomly adds / changes a child of node
	void crossover(std::mt19937& random, Node* buffer_a, Node* buffer_b); // Swaps two subtrees of a and b
}
