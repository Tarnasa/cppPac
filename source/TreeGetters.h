// Contains functions which return some subset of a tree

#pragma once

#include "Node.h"

namespace Brain
{
	std::vector<Node*> get_parent_nodes(Node* node);
	void _get_parent_nodes_recursive(std::vector<Node*>& nodes, Node* node);
	std::vector<Node*> subtree(Node* node);
	void _subtree_recursive(std::vector<Node*>& nodes, Node* node);

	Node* get_random_parent_node(std::mt19937& random, Node* root);

	int count_levels(const Node* node);
	int count_nodes(const Node* node);

	bool equal(const Node* a, const Node* b);
}
