#include "Index.h"
#include <malloc.h>
/*
 * Initialize a given branch
 */
void init_Branch(struct Branch *branch)
{
	init_Rect(&(branch->rect));
	branch->child = NULL;
}
/*
 * Initialize a given Node
 */
void init_Node(struct Node* Node)
{
	register struct Node* nodeR = Node;
	register int i;
	nodeR->count = 0;
	nodeR->level = -1;
	for(i=0;i<BRANCH_FACTOR;i++)
	{
		init_Branch(&(nodeR->branch[i]));
	}
}
/*
 * Allocates and initializes a new node
 */
struct Node* newNode()
{
	register struct Node* nR;
	nR = (struct Node*)malloc(sizeof(struct Node));
	init_Node(nR);
	return nR;
}
/*
 * frees a given node
 */
void freeNode(struct Node *nodePointer)
{
	free(nodePointer);
}
/*
 * ChooseLeaf algorithm [mentioned in the original paper] implementation
 * Given a rectangle and a particular node, select the best branch
 * Area calculated by using Spherical volume formula to avoid anomalies
 * [proposed by Paul Brooke]
 */
int pickBranch(struct Rectangle *targetRect, struct Node *currNode)
{
	register int i, flag = 1;
	register struct Rectangle *tarRect_R = targetRect;
	register struct Node *curNode_R = currNode;
	register struct Rectangle *br;
	float improvement, bestImprovement=-1, area, bestArea;
	struct Rectangle temp;
	int best_index;
	for(i=0;i<BRANCH_FACTOR;i++)
	{
		if(curNode_R->branch[i].child)
		{
			br = &curNode_R->branch[i].rect;
			area = rectSphericalVol(br);
			temp = combineRect(tarRect_R,br);
			//Defined as the difference between the area of combined bounding rectangle and the given rectangle
			improvement = rectSphericalVol(&temp)-area;
			if(improvement < bestImprovement || flag)
			{
				best_index = i;
				bestArea = area;
				bestImprovement = improvement;
				flag = 0;
			}
			else if (improvement == bestImprovement && area<bestArea)
			{
				best_index = i;
				bestArea = area;
				bestImprovement = improvement;
			}
		}
	}
	return best_index;
}
/*
 * Insert a particular branch in a node
 * If the branch array is full in the node, split the node.
 * return saying the the node is split
 */
int addBranchToNode(struct Branch *branchEntry, struct Node *targetNode, struct Node **New_node)
{
	register struct Branch *branchEntry_R = branchEntry;
	register struct Node *targetNode_R = targetNode;
	register struct Node **new_node = New_node;
	register int i;
	if(targetNode_R->count<BRANCH_FACTOR)
	{
		for(i=0;i<BRANCH_FACTOR;i++)
		{
			if(targetNode_R->branch[i].child == NULL)
			{
				targetNode_R->branch[i] = *branchEntry_R;
				targetNode_R->count++;
				break;
			}
		}
		return 0;
	}
	else
	{
		split_node_ls(targetNode_R, branchEntry_R, new_node);
		return 1;
	}
}
/*
 * Find the bounding rectangle of the branch whose child is N
 */
struct Rectangle findCoveringRect(struct Node *targetNode)
{
	register int i, flag=1;
	register struct Node *nR = targetNode;
	struct Rectangle coveringRect;
	init_Rect(&coveringRect);
	for (i = 0; i < BRANCH_FACTOR; i++)
	{
		if (nR->branch[i].child)
		{
			if (flag)
			{
				coveringRect = nR->branch[i].rect;
				flag = 0;
			}
			else
			{
				coveringRect = combineRect(&coveringRect, &(nR->branch[i].rect));
			}
		}
	}
	return coveringRect;
}
void disConnectBranch(struct Node *n, int i)
{
	init_Branch(&(n->branch[i]));
	n->count--;
}
/*
 * Print out the data in a node.
 */
void printNodes(struct Node *currNode, int depth)
{
	int i;
	provideTabSpace(depth);
	printf("node");
	if (currNode->level == 0)
		printf(" LEAF");
	else if (currNode->level > 0)
		printf(" NONLEAF");
	else
		printf(" TYPE=?");
	printf("  level=%d  count=%d  address=%p\n", currNode->level, currNode->count, currNode);

	for (i=0; i<currNode->count; i++)
	{
		if(currNode->level == 0) {
			provideTabSpace(depth);
			printf("\t%d: data = %d\n", i, currNode->branch[i].child);
		}
		else
		{
			provideTabSpace(depth);
			printf("branch %d\n", i);
			RTreePrintBranch(&currNode->branch[i], depth+1);
		}
	}
}
/*
 * Print out particular branch data
 */
void RTreePrintBranch(struct Branch *b, int depth)
{
	print_Rectangle(&(b->rect), depth);
	printNodes(b->child, depth);
}
/*
 * Provides intendation for printing
 */
extern void provideTabSpace(int depth)
{
	int i;
	for(i=0; i<depth; i++)
		putchar('\t');
}
