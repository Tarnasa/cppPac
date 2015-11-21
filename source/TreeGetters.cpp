#include "TreeGetters.h"

namespace Brain
{
	std::vector<Node*> getNonLeaves(Node* node)
	{
		std::vector<Node*> nodes;
		if (node == nullptr) return nodes;
		_getNonLeavesRecursive(nodes, node);
		return nodes;
	}

	void _getNonLeavesRecursive(std::vector<Node*>& nodes, Node* node)
	{
		if (node->children.size() > 0)
		{
			nodes.emplace_back(node);
			for (Node* child : node->children)
			{
				_getNonLeavesRecursive(nodes, child);
			}
		}
	}

	std::vector<Node*> getOpenNodes(Node* node) // Nodes that can accept another child
	{
		std::vector<Node*> nodes;
		if (node == nullptr) return nodes;
		return nodes;
	}

	void _getOpenNodesRecursive(std::vector<Node*>& nodes, Node* node)
	{
		if (node->children.size() < node->GetMaxChildren())
			nodes.emplace_back(node);
		for (Node* child : node->children)
		{
			_getOpenNodesRecursive(nodes, child);
		}
	}

	std::vector<Node*> subtree(Node* root)
	{
		std::vector<Node*> nodes;
		for (Node* child : root->children)
		{
			_subtreeRecursive(nodes, root);
		}
		return nodes;
	}

	void _subtreeRecursive(std::vector<Node*>& nodes, Node* node)
	{
		if (node == nullptr) return;
		nodes.emplace_back(node);
		for (Node* child : node->children)
		{
			_subtreeRecursive(nodes, child);
		}
	}

	Node* getRandomNonLeaf(std::mt19937& random, Node* root)
	{
		std::vector<Node*> nodes = getNonLeaves(root);
		return nodes[std::uniform_int_distribution<int>(0, nodes.size() - 1)(random)];
	}

	int count_levels(Node* node)
	{
		if (node == nullptr) return 0;
		int height = 1;
		for (Node* child : node->children)
		{
			int child_height = count_levels(child) + 1;
			if (child_height > height)
			{
				height = child_height;
			}
		}
		return height;
	}

	int count_nodes(Node* node)
	{
		if (node == nullptr) return 0;
		int nodes = 1;
		for (Node* child : node->children)
		{
			nodes += count_nodes(child);
		}
		return nodes;
	}
}
