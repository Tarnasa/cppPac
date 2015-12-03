#include "TreeOperators.h"

#include <vector>

#include "TreeGetters.h"
#include "Helpers.h"
#include "TreeGenerators.h"

namespace Brain
{
	Node* copy_tree(const Node* root)
	{
		Node* copy = root->Clone();
		for (size_t i = 0; i < copy->children.size(); ++i)
		{
			copy->children[i] = copy_tree(copy->children[i]);
		}
		return copy;
	}

	void replace_one_node(std::mt19937& random, Node*& node, bool is_ghost) // Replaces one random node in the subtree
	{
		if (node == nullptr) return;
		if (node->children.size() == 0)
		{
			// Replace this one
			delete node;
			node = generate_terminal_node(random, is_ghost);
		}
		else
		{
			// Choose a random node in the subtree
			auto parents = get_parent_nodes(node);
			if (chance(random, 1.0 / (parents.size() + 1.0)))
			{
				// Replace this one
				Node* old_node = node;

				node = generate_parent_node(random);
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
					parent->children[child_index] = generate_terminal_node(random, is_ghost);
				}
				else
				{
					parent->children[child_index] = generate_parent_node(random);
					parent->children[child_index]->children = child->children;
					child->children.clear(); // Don't kill the babies
				}
				delete child;
			}
		}
	}

	void replace(std::mt19937& random, Node*& node, bool is_ghost) // Replaces node with a new tree of equal height
	{
		int height = count_levels(node);
		delete node;
		node = generate_tree(random, height, is_ghost);
	}

	void grow(std::mt19937& random, Node*& node, int max_levels, bool is_ghost) // Replaces a node with a new tree of possibly greater height
	{
		if (node->children.size() == 0)
		{
			delete node;
			node = generate_tree_up_to(random, max_levels, is_ghost);
		}
		else
		{
			Node* parent = get_random_parent_node(random, node);
			grow(random, parent->children[random_int(random, 0, parent->children.size() - 1)], max_levels, is_ghost);
		}
	}

	void mutate(std::mt19937& random, BufferNode* root, int grow_max_levels, bool is_ghost) // Applies either replace one node or grow
	{
		if (chance(random, 0.5))
		{
			replace_one_node(random, root->children[0], is_ghost);
		}
		else
		{
			grow(random, root->children[0], grow_max_levels, is_ghost);
		}
	}

	void crossover(std::mt19937& random, Node* buffer_a, Node* buffer_b) // Swaps two subtrees of a and b
	{
		Node* a_parent = get_random_parent_node(random, buffer_a);
		Node* b_parent = get_random_parent_node(random, buffer_b);

		int a_child_index = random_int(random, 0, a_parent->children.size() - 1);
		int b_child_index = random_int(random, 0, b_parent->children.size() - 1);

		if (buffer_a == buffer_b && a_parent == b_parent && a_child_index == b_child_index) return;
		Node* temp = a_parent->children[a_child_index];
		a_parent->children[a_child_index] = b_parent->children[b_child_index];
		b_parent->children[b_child_index] = temp;
	}
}
