#include "TreeOperators.h"

#include <vector>

#include "TreeGetters.h"
#include "Helpers.h"
#include "TreeGenerators.h"

namespace Brain
{
	void swapRandomChildren(std::mt19937& random, Node* a, Node* b)
	{
		// Assumes that a and b have children
		int child_a = std::uniform_int_distribution<int>(0, a->children.size() - 1)(random);
		int child_b = std::uniform_int_distribution<int>(0, b->children.size() - 1)(random);
		Node* temp = a->children[child_a];
		a->children[child_a] = b->children[child_b];
		b->children[child_b] = temp;
	}

	Node* createCopy(Node* root)
	{
		Node* copy = root->Clone();
		for (int i = 0; i < copy->children.size(); ++i)
		{
			copy->children[i] = createCopy(copy->children[i]);
		}
		return copy;
	}

	void replaceOneNode(std::mt19937& random, Node*& node) // Replaces one random node in the subtree
	{
		if (node == nullptr) return;
		if (node->children.size() == 0)
		{
			// Replace this one
			delete node;
			node = generateTerminalNode(random);
		}
		else
		{
			// Choose a random node in the subtree
			auto parents = getNonLeaves(node);
			if (chance(random, 1.0 / (parents.size() + 1.0)))
			{
				// Replace this one
				Node* old_node = node;

				node = generateParentNode(random);
				node->children = old_node->children;

				old_node->children.clear(); // Don't delete children
				delete old_node;
			}
			else
			{
				// Replace one of parent's children
				Node* parent = choice(random, parents);
				int child_index = random_int(random, 0, parent->children.size() - 1);
				Node* child = parent->children[child_index];
				if (child->children.size() == 0)
				{
					parent->children[child_index] = generateTerminalNode(random);
				}
				else
				{
					parent->children[child_index] = generateParentNode(random);
					parent->children[child_index]->children = child->children;
					child->children.clear(); // Don't kill the babies
				}
				delete child;
			}
		}
	}

	void replace(std::mt19937& random, Node*& node) // Replaces node with a new tree of equal height
	{
		int height = getHeight(node);
		delete node;
		node = generateTree(random, height);
	}

	void grow(std::mt19937& random, Node*& node, int max_levels) // Replaces a leaf node with a new tree of possibly greater height
	{
		if (node->children.size() == 0)
		{
			delete node;
			node = generateTreeUpTo(random, max_levels);
		}
		else
		{
			Node* parent = getRandomNonLeaf(random, node);
			grow(random, parent->children[random_int(random, 0, parent->children.size() - 1)], max_levels);
		}
	}

	void mutate(std::mt19937& random, BufferNode* root) // Applies either replace one node or grow
	{
		if (chance(random, 1.0))
		{
			replaceOneNode(random, root->children[0]);
		}
		else
		{
			grow(random, root->children[0], 3);
		}
	}

	void crossover(std::mt19937& random, Node* buffer_a, Node* buffer_b) // Swaps two subtrees of a and b
	{
		Node* a_parent = getRandomNonLeaf(random, buffer_a);
		Node* b_parent = getRandomNonLeaf(random, buffer_b);

		int a_child_index = random_int(random, 0, a_parent->children.size() - 1);
		int b_child_index = random_int(random, 0, b_parent->children.size() - 1);

		if (buffer_a == buffer_b && a_parent == b_parent && a_child_index == b_child_index) return;
		Node* temp = a_parent->children[a_child_index];
		a_parent->children[a_child_index] = b_parent->children[b_child_index];
		b_parent->children[b_child_index] = temp;
	}

	/*
	void mutatexx(std::mt19937& random, Node* node)
	{
		if (node == nullptr) return;

		if (std::uniform_real_distribution<double>(0, 1)(random) < append_chance)
		{
			if (node->children.size() < node->GetMaxChildren())
			{
				node->children.emplace_back(createRandomNode(random));
			}
		}
		if (std::uniform_real_distribution<double>(0, 1)(random) < remove_chance)
		{
			if (node->children.size() > node->GetMinChildren())
			{
				int last = node->children.size() - 1;
				int index = std::uniform_int_distribution<int>(0, last)(random);
				delete node->children[index];
				node->children[index] = node->children[last];
				node->children.pop_back();
			}
		}
		if (std::uniform_real_distribution<double>(0, 1)(random) < swap_chance)
		{
			// Find the first node with more than one child
			Node* runner = node;
			while (node->children.size() == 1)
				runner = runner->children[0];
			if (runner->children.size() != 0)
			{
				// Swap two random non-leaves from separate branches
				Node* subtree_a = choice(random, runner->children);
				Node* subtree_b = choice(random, runner->children);
				while (subtree_a == subtree_b)
					subtree_b = choice(random, runner->children);
				Node* parent_a = getRandomNonLeaf(random, subtree_a);
				Node* parent_b = getRandomNonLeaf(random, subtree_b);
				swapRandomChildren(random, parent_a, parent_b);
			}
		}
		if (std::uniform_int_distribution<double>(0, 1)(random) < duplication_chance)
		{
			Node* parent = choice(random, getOpenNodes(node));
			parent->children.emplace_back(createCopy(choice(random, subtree(node))));
		}
	}
	*/
}
