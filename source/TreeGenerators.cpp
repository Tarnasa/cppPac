#include "TreeGenerators.h"

#include <assert.h>

#include "Helpers.h"

namespace Brain
{
	Node* generateTree(std::mt19937& random, int levels)
	{
		if (levels <= 0) return nullptr;
		if (levels == 1)
		{
			return generateTerminalNode(random);
		}
		else
		{
			Node* parent = generateParentNode(random);
			// Make first child have levels - 1 levels in order to gaurantee `levels` levels
			parent->children.emplace_back(generateTree(random, levels - 1));
			// Make other children have random between 1, and levels - 1 levels
			for (int child_index = 1; child_index < parent->GetMinChildren(); ++child_index)
			{
				parent->children.emplace_back(generateTree(random, random_int(random, 1, levels - 1)));
			}
			return parent;
		}
	}

	Node* generateFullTree(std::mt19937& random, int levels)
	{
		if (levels <= 0) return nullptr;
		if (levels == 1)
		{
			return generateTerminalNode(random);
		}
		else
		{
			Node* parent = generateParentNode(random);
			for (int child_index = 0; child_index < parent->GetMinChildren(); ++child_index)
			{
				parent->children.emplace_back(generateFullTree(random, levels - 1));
			}
			return parent;
		}
	}

	Node* generateTreeUpTo(std::mt19937& random, int max_levels)
	{
		if (max_levels <= 0) return nullptr;
		if (max_levels == 1)
		{
			return generateTerminalNode(random);
		}
		else
		{
			Node* parent = generateParentNode(random);
			for (int child_index = 0; child_index < parent->GetMinChildren(); ++child_index)
			{
				parent->children.emplace_back(generateTree(random, random_int(random, 1, max_levels - 1)));
			}
			return parent;
		}
	}

	Node* generateTerminalNode(std::mt19937& random)
	{
		switch (random_int(random, 0, 3))
		{
		case 0: return createNodeFromInt(random, Constant); break;
		case 1: return createNodeFromInt(random, PacmanToDot); break;
		case 2: return createNodeFromInt(random, PacmanToGhost); break;
		case 3: return createNodeFromInt(random, PacmanDotsEaten); break;
		}
	}

	Node* generateParentNode(std::mt19937& random)
	{
		switch (random_int(random, 0, 5))
		{
		case 0: return createNodeFromInt(random, Sum); break;
		case 1: return createNodeFromInt(random, Subtract); break;
		case 2: return createNodeFromInt(random, Product); break;
		case 3: return createNodeFromInt(random, Divide); break;
		case 4: return createNodeFromInt(random, RandomRange); break;
		}
	}

}
