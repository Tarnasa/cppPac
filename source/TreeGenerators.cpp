#include "TreeGenerators.h"

#include "Helpers.h"

namespace Brain
{
	Node* generate_tree(std::mt19937& random, int levels, bool is_ghost)
	{
		if (levels <= 0) return nullptr;
		if (levels == 1)
		{
			return generate_terminal_node(random, is_ghost);
		}
		else
		{
			Node* parent = generate_parent_node(random);
			// Make first child have levels - 1 levels in order to gaurantee `levels` levels
			parent->children.emplace_back(generate_tree(random, levels - 1, is_ghost));
			// Make other children have random between 1, and levels - 1 levels
			for (int child_index = 1; child_index < parent->GetMinChildren(); ++child_index)
			{
				parent->children.emplace_back(generate_tree(random, random_int(random, 1, levels - 1), is_ghost));
			}
			return parent;
		}
	}

	Node* generate_full_tree(std::mt19937& random, int levels, bool is_ghost)
	{
		if (levels <= 0) return nullptr;
		if (levels == 1)
		{
			return generate_terminal_node(random, is_ghost);
		}
		else
		{
			Node* parent = generate_parent_node(random);
			for (int child_index = 0; child_index < parent->GetMinChildren(); ++child_index)
			{
				parent->children.emplace_back(generate_full_tree(random, levels - 1, is_ghost));
			}
			return parent;
		}
	}

	Node* generate_tree_up_to(std::mt19937& random, int max_levels, bool is_ghost)
	{
		if (max_levels <= 0) return nullptr;
		if (max_levels == 1)
		{
			return generate_terminal_node(random, is_ghost);
		}
		else
		{
			Node* parent = generate_parent_node(random);
			for (int child_index = 0; child_index < parent->GetMinChildren(); ++child_index)
			{
				parent->children.emplace_back(generate_tree(random, random_int(random, 1, max_levels - 1), is_ghost));
			}
			return parent;
		}
	}

	Node* generate_terminal_node(std::mt19937& random, bool is_ghost)
	{
		if (is_ghost)
		{
			switch (random_int(random, 0, 2))
			{
			case 0: return createNodeFromInt(random, Constant); break;
			case 1: return createNodeFromInt(random, GhostToGhost); break;
			case 2: return createNodeFromInt(random, GhostToPacman); break;
			}
		}
		else
		{
			switch (random_int(random, 0, 3))
			{
			case 0: return createNodeFromInt(random, Constant); break;
			case 1: return createNodeFromInt(random, PacmanToDot); break;
			case 2: return createNodeFromInt(random, PacmanToGhost); break;
			case 3: return createNodeFromInt(random, PacmanDotsEaten); break;
			}
		}
		return nullptr;
	}

	Node* generate_parent_node(std::mt19937& random)
	{
		switch (random_int(random, 0, 4))
		{
		case 0: return createNodeFromInt(random, Sum); break;
		case 1: return createNodeFromInt(random, Subtract); break;
		case 2: return createNodeFromInt(random, Product); break;
		case 3: return createNodeFromInt(random, Divide); break;
		case 4: return createNodeFromInt(random, RandomRange); break;
		}
		return nullptr;
	}
}
