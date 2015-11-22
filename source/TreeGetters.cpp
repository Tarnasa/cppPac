#include "TreeGetters.h"

namespace Brain
{
	std::vector<Node*> get_parent_nodes(Node* node)
	{
		std::vector<Node*> nodes;
		if (node == nullptr) return nodes;
		_get_parent_nodes_recursive(nodes, node);
		return nodes;
	}

	void _get_parent_nodes_recursive(std::vector<Node*>& nodes, Node* node)
	{
		if (node->children.size() > 0)
		{
			nodes.emplace_back(node);
			for (Node* child : node->children)
			{
				_get_parent_nodes_recursive(nodes, child);
			}
		}
	}

	std::vector<Node*> subtree(Node* root)
	{
		std::vector<Node*> nodes;
		for (Node* child : root->children)
		{
			_subtree_recursive(nodes, root);
		}
		return nodes;
	}

	void _subtree_recursive(std::vector<Node*>& nodes, Node* node)
	{
		if (node == nullptr) return;
		nodes.emplace_back(node);
		for (Node* child : node->children)
		{
			_subtree_recursive(nodes, child);
		}
	}

	Node* get_random_parent_node(std::mt19937& random, Node* root)
	{
		std::vector<Node*> nodes = get_parent_nodes(root);
		return nodes[std::uniform_int_distribution<int>(0, nodes.size() - 1)(random)];
	}

	int count_levels(const Node* node)
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

	int count_nodes(const Node* node)
	{
		if (node == nullptr) return 0;
		int nodes = 1;
		for (Node* child : node->children)
		{
			nodes += count_nodes(child);
		}
		return nodes;
	}

	bool equal(const Node* a, const Node* b)
	{
		if (a == b) return true;
		if (a == nullptr || b == nullptr) return false;
		if (a->GetChar() != b->GetChar()) return false;
		if (a->children.size() != b->children.size()) return false;
		for (size_t i = 0; i < a->children.size(); ++i)
		{
			if (!equal(a->children[i], b->children[i])) return false;
		}
		return true;
	}
}
