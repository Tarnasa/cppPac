// Contains functions which return some subset of a tree

#pragma once

#include "Node.h"

namespace Brain
{
	std::vector<Node*> getNonLeaves(Node* node);
	void _getNonLeavesRecursive(std::vector<Node*>& nodes, Node* node);
	std::vector<Node*> getOpenNodes(Node* node);
	void _getOpenNodesRecursive(std::vector<Node*>& nodes, Node* node);
	std::vector<Node*> subtree(Node* node);
	void _subtreeRecursive(std::vector<Node*>& nodes, Node* node);

	Node* getRandomNonLeaf(std::mt19937& random, Node* root);

	int getHeight(Node* node);
}
