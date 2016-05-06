#include<stdio.h>
#include <malloc.h>
#include "Index.h"
/*
 * Creates a new R-Tree Index
 */
struct Node* newRTreeIndex()
{
	struct Node* root;
	root = newNode();
	root->level =0;
	return root;
}
/*
 * Contains a list of nodes.
 * Used during deletion for reinsert
 */
static struct ListNode * newListNode()
{
	return (struct ListNode *) malloc(sizeof(struct ListNode));
}
/*
 * Free up link list of nodes
 */
static void freeListNode(struct ListNode *Node)
{
	free(Node);
}
/*
 * Add a node that needs to be reinserted into the list
 */
static void addReInsertNode(struct Node *reInsertNode, struct ListNode **reInsertList)
{
	register struct ListNode *l;
	l = newListNode();
	l->node = reInsertNode;
	l->next = *reInsertList;
	*reInsertList = l;
}

/*void kNNQuery(struct Node *curNode, struct Rectangle *R, UserDefFunction cPrint, void* cbarg, int k)
{
	register struct Node *n = curNode;
	register struct Rectangle *r = R;
	register int resultCount = 0;
}*/

int containment_Search(struct Node *curNode, struct Rectangle *R, UserDefFunction cPrint, void* cbarg)
{
	register struct Node *n = curNode;
	register struct Rectangle *r = R;
	register int resultCount = 0;
	register int i;
	//Internal Node
	if (n->level > 0)
	{
		for (i=0; i<BRANCH_FACTOR; i++)
		{
			if (n->branch[i].child && checkIfOverlaps(&n->branch[i].rect,r))
			{
				resultCount += containment_Search(n->branch[i].child, R, cPrint, cbarg);
			}
		}
	}
	//leaf node
	else
	{
		for (i=0; i<BRANCH_FACTOR; i++)
		{
			if (n->branch[i].child && isContained(&n->branch[i].rect,r))
			{
				resultCount++;
				//User defined callback function
				if(cPrint)
				{
					//Check if callback wants to discontinue
					if(!cPrint((int)n->branch[i].child, cbarg))
					{
						return resultCount;
					}
				}
			}
		}
	}
	return resultCount;
}
int intersection_Search(struct Node *N, struct Rectangle *R, UserDefFunction cPrint, void* cbarg)
{
	register struct Node *n = N;
	register struct Rectangle *r = R;
	register int hitCount = 0;
	register int i;
	//Internal Node
	if (n->level > 0)
	{
		for (i=0; i<BRANCH_FACTOR; i++)
		{
			if (n->branch[i].child && checkIfOverlaps(r,&n->branch[i].rect))
			{
				hitCount += intersection_Search(n->branch[i].child, R, cPrint, cbarg);
			}
		}
	}
	//leaf node
	else
	{
		for (i=0; i<BRANCH_FACTOR; i++)
		{
			if (n->branch[i].child && checkIfOverlaps(r,&n->branch[i].rect))
			{
				hitCount++;
				//User defined callback function
				if(cPrint)
				{
					if( ! cPrint((int)n->branch[i].child, cbarg)) //Check if callback wants to discontinue
					{
						return hitCount;
					}
				}
			}
		}
	}
	return hitCount;
}
static int insertion_part2(struct Rectangle *r, int tid, struct Node *n, struct Node **new_node, int level)
{
	register int i;
	struct Branch b;
	struct Node *n2;
	//Recursive call when level greater than insertion level
	if (n->level > level)
	{
		i = pickBranch(r, n);
		//Child node not split
		if (!insertion_part2(r, tid, n->branch[i].child, &n2, level))
		{
			n->branch[i].rect = combineRect(r,&(n->branch[i].rect));
			return 0;
		}
		else    // child split
		{
			n->branch[i].rect = findCoveringRect(n->branch[i].child);
			b.child = n2;
			b.rect = findCoveringRect(n2);
			return addBranchToNode(&b, n, new_node);
		}
	}
	//Level of insertion
	else if (n->level == level)
	{
		b.rect = *r;
		b.child = (struct Node *) tid;
		//child field of leaves contains tid
		return addBranchToNode(&b, n, new_node);
	}
	else
	{
		return 0;
	}
}
int insertRectangle(struct Rectangle *R, int Tid, struct Node **Root, int Level)
{
	register struct Rectangle *r = R;
	register int tid = Tid;
	register struct Node **root = Root;
	register int level = Level;
	register struct Node *newroot;
	struct Node *newnode;
	struct Branch b;
	int result;
	//Root split scenario
	if (insertion_part2(r, tid, *root, &newnode, level))
	{
		newroot = newNode();
		newroot->level = (*root)->level + 1;
		b.rect = findCoveringRect(*root);
		b.child = *root;
		addBranchToNode(&b, newroot, NULL);
		b.rect = findCoveringRect(newnode);
		b.child = newnode;
		addBranchToNode(&b, newroot, NULL);
		*root = newroot;
		result = 1;
	}
	else
		result = 0;
	return result;
}
static int deleteNode_part2(struct Rectangle *R, int Tid, struct Node *N, struct ListNode **Ee)
{
	register struct Rectangle *r = R;
	register int tid = Tid;
	register struct Node *n = N;
	register struct ListNode **ee = Ee;
	register int i;
	register int j = BRANCH_FACTOR/2;
	if (n->level > 0)  // not a leaf node
	{
	    for (i = 0; i < BRANCH_FACTOR; i++)
	    {
	    	if (n->branch[i].child && checkIfOverlaps(r, &(n->branch[i].rect)))
	    	{
	    		if (!deleteNode_part2(r, tid, n->branch[i].child, ee))
	    		{
	    			if (n->branch[i].child->count >= j)
	    				n->branch[i].rect = findCoveringRect(n->branch[i].child);
	    			else
	    			{
	    				//Entries less than minFill Remove child and reinsert branches in R-Tree
	    				addReInsertNode(n->branch[i].child, ee);
	    				disConnectBranch(n, i);
	    			}
	    			return 0;
	    		}
	    	}
	    }
	    return 1;
	}
	else  // a leaf node
	{
		for (i = 0; i < BRANCH_FACTOR; i++)
		{
			if (n->branch[i].child && n->branch[i].child == (struct Node *) tid)
			{
				disConnectBranch(n, i);
				return 0;
			}
		}
		return 1;
	}
}
int deleteNode(struct Rectangle *R, int Tid, struct Node**Nn)
{
	register struct Rectangle *r = R;
	register int tid = Tid;
	register struct Node **nn = Nn;
	register int i;
	register struct Node *tmp_nptr;
	struct ListNode *reInsertList = NULL;
	register struct ListNode *e;
	//Found the data item and deleted it
	if (!deleteNode_part2(r, tid, *nn, &reInsertList))
	{
		//Reinsert eliminated nodes if any
		while (reInsertList)
		{
			tmp_nptr = reInsertList->node;
			for (i = 0; i < BRANCH_FACTOR; i++)
			{
				if (tmp_nptr->branch[i].child)
				{
					insertRectangle(&(tmp_nptr->branch[i].rect),(int)tmp_nptr->branch[i].child,nn,tmp_nptr->level);
				}
			}
			e = reInsertList;
			reInsertList = reInsertList->next;
			freeNode(e->node);
			freeListNode(e);
		}
		//Redundant root -- Not a leaf node and with one child
		if ((*nn)->count == 1 && (*nn)->level > 0)
		{
			for (i = 0; i < BRANCH_FACTOR; i++)
			{
				tmp_nptr = (*nn)->branch[i].child;
				if(tmp_nptr)
					break;
			}
			freeNode(*nn);
			*nn = tmp_nptr;
		}
		return 0;
	}
	else
	{
		return 1;
	}
}
