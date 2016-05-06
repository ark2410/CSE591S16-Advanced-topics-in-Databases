#include "linear_split.h"
#include <stdio.h>
#include "Index.h"

/*
 * This method is used to append all the branches considered in the split to the branch buffer
 * A covering rectangle for all the branches (including the overflow one) is computed
 * The old node is reinitialized
 */
static void getSplitNodeBranches (struct Node *splitNode, struct Branch *extra_Branch)
{
	register struct Node *splitNode_r = splitNode;
	register struct Branch *extraBranch_r = extra_Branch;
	register int i;
	// Add BRANCH_FACTOR nodes in a Buffer
	for (i=0;i<BRANCH_FACTOR;i++)
		BranchBuf[i]=splitNode_r->branch[i];
	//Add the extra new branch
	BranchBuf[BRANCH_FACTOR]=*extraBranch_r;
	BranchCount = BRANCH_FACTOR+1;
	// Find a covering rectangle for for all branches
	CoverSplit = BranchBuf[0].rect;
	for (i=1;i<BranchCount;i++)
		CoverSplit = combineRect(&CoverSplit,&BranchBuf[i].rect);
	//Reinitialize the old node
	init_Node(splitNode_r);
}
/*
 * Initialize all the variables in struct partition
 */
static void initPartitionVariables(struct PartitionVars *partitionStruct, int branchCount_new, int minfill)
{
	register struct PartitionVars *partitionVars_r = partitionStruct;
	register int i;
	partitionVars_r->count[0] = partitionVars_r->count[1] = 0;
	partitionVars_r->total = branchCount_new;
	partitionVars_r->minfill = minfill;
	for (i=0; i<branchCount_new; i++)
	{
		partitionVars_r->isAssigned[i] = 0;
		partitionVars_r->assignedTo[i] = -1;
	}
}
/*
 * Updating partition variables when adding a node to a group.
 * 	1. Update isAssigned and assignedTo variables
 *	2. Find the new covering rectangle for the group
 *	3. Increment the group count
 */
static void updatePartitions(int i, int group, struct PartitionVars *partitionStruct)
{
	partitionStruct->assignedTo[i] = group;
	partitionStruct->isAssigned[i] = 1;

	if (partitionStruct->count[group] == 0)
		partitionStruct->cover[group] = BranchBuf[i].rect;
	else
		partitionStruct->cover[group] = combineRect(&BranchBuf[i].rect,&partitionStruct->cover[group]);
	partitionStruct->area[group] = rectSphericalVol(&partitionStruct->cover[group]);
	partitionStruct->count[group]++;
}
/*
 * Linear Pick Seeds algorithm present in the original paper.
*/
static void pickSeeds(struct PartitionVars *partitionStruct)
{
	register struct PartitionVars *p = partitionStruct;
	register int i;
	register struct Rectangle *r, *lowRect, *highRect;
	register float w, separation, bestSep;
	float width[2];
	int leastUpper[2], greatestLower[2];
	int bIndexForNode0, bIndexForNode1;
	/*
	 * Find rectangles that has the least high coordinate and greatest low coordinate in each dimension
	 */
	greatestLower[0]=greatestLower[1]=leastUpper[0]=leastUpper[1]=0;
	for (i=1; i<BRANCH_FACTOR+1; i++)
	{
		r = &BranchBuf[i].rect;
		if(r->boundary[0]>BranchBuf[greatestLower[0]].rect.boundary[0])
			greatestLower[0]=i;
		if(r->boundary[1]>BranchBuf[greatestLower[1]].rect.boundary[1])
			greatestLower[1]=i;
		if (r->boundary[2] < BranchBuf[leastUpper[0]].rect.boundary[2])
			leastUpper[0] = i;
		if (r->boundary[3] < BranchBuf[leastUpper[1]].rect.boundary[3])
			leastUpper[0] = i;
	}
	/*
	 * Calculate height and width of covering boundary
	 */
	width[0]=CoverSplit.boundary[2]-CoverSplit.boundary[0];
	width[1]=CoverSplit.boundary[3]-CoverSplit.boundary[1];

	/*
	 * Pick rectangle pair along the dimension with maximum separation
	 */

	//Along X coordinate
	w = width[0] ==0?(float)1:width[0];
	lowRect = &BranchBuf[leastUpper[0]].rect;
	highRect = &BranchBuf[greatestLower[0]].rect;
	bIndexForNode0 = leastUpper[0];
	bIndexForNode1 = greatestLower[0];
	separation = bestSep = (highRect->boundary[0] - lowRect->boundary[2])/w;

	//Along Y coordinate [Replace bestSep if its better than X dimension]
	w = width[1] ==0?(float)1:width[1];
	lowRect = &BranchBuf[leastUpper[1]].rect;
	highRect = &BranchBuf[greatestLower[1]].rect;
	separation = (highRect->boundary[1] -lowRect->boundary[3]) / w;
	if (separation > bestSep)
	{
		bIndexForNode0 = leastUpper[1];
		bIndexForNode1 = greatestLower[1];
		bestSep = separation;
	}
	//Add the branches to each node as long as the distance between them in both dimensions are not the same
	if (bIndexForNode0 != bIndexForNode1)
	{
		updatePartitions(bIndexForNode0, 0, p);
		updatePartitions(bIndexForNode1, 1, p);
	}
}
/*
 * Algorithm to place the remaining BRANCH_FACTOR-2+1 branches between 2 nodes
 */
static void afterPickSeeds(struct PartitionVars *partitionStruct)
{
	register struct PartitionVars *partitionVars_local = partitionStruct;
	struct Rectangle newTempCover[2];
	register int i, group;
	float newTempArea[2], increase[2];
	for (i=0; i<BRANCH_FACTOR+1; i++)
	{
		if (!partitionVars_local->isAssigned[i])
		{
			//One group too full that other group would go below minfill
			if (partitionVars_local->count[0] >= partitionVars_local->total - partitionVars_local->minfill)
			{
				updatePartitions(i, 1, partitionVars_local);
				continue;
			}
			else if (partitionVars_local->count[1] >= partitionVars_local->total - partitionVars_local->minfill)
			{
				updatePartitions(i, 0, partitionVars_local);
				continue;
			}
			//Find increase in area for the rectangle with 2 groups
			for (group=0; group<2; group++)
			{
				if (partitionVars_local->count[group]>0)
					newTempCover[group] = combineRect(&BranchBuf[i].rect,&partitionVars_local->cover[group]);
				else
					newTempCover[group] = BranchBuf[i].rect;
				newTempArea[group] = rectSphericalVol(&newTempCover[group]);
				increase[group] = newTempArea[group]-partitionVars_local->area[group];
			}
			//If increase is not the same for 2 groups add to the group that increases least.
			if (increase[0] < increase[1])
				updatePartitions(i, 0, partitionVars_local);
			else if (increase[1] < increase[0])
				updatePartitions(i, 1, partitionVars_local);
			//If increase is same, add to group that has lesser area
			else if (partitionVars_local->area[0] < partitionVars_local->area[1])
				updatePartitions(i, 0, partitionVars_local);
			else if (partitionVars_local->area[1] < partitionVars_local->area[0])
				updatePartitions(i, 1, partitionVars_local);

			//Else put it in group that has less number of elements
			else if (partitionVars_local->count[0] < partitionVars_local->count[1])
				updatePartitions(i, 0, partitionVars_local);
			//Randomly pick the second one
			else
				updatePartitions(i, 1, partitionVars_local);
		}
	}
}
/*
 * function that initializes partition structure, runs pickseeds and afterPickseeds
 */
static void split_starter(struct PartitionVars *partitionStruct, int minfill)
{
	initPartitionVariables(partitionStruct, BranchCount, minfill);
	pickSeeds(partitionStruct);
	afterPickSeeds(partitionStruct);
}
/*
 * This method assigns each branch to its respective nodes
 */
static void finishPartition (struct Node *oldNode, struct Node *secNode, struct PartitionVars *partitionStruct)
{
	register struct Node *oldNode_r = oldNode, *newNode_r = secNode;
	register struct PartitionVars *partition_r = partitionStruct;
	register int i;
	for (i=0; i<BRANCH_FACTOR+1; i++)
	{
		if (partition_r->assignedTo[i] == 0)
			addBranchToNode(&BranchBuf[i], oldNode_r, NULL);
		else if (partition_r->assignedTo[i] == 1)
			addBranchToNode(&BranchBuf[i], newNode_r, NULL);
	}
}
/*
 * Starter function for splitting nodes using Linear Split
 * 	1. Add branches to buffer
 * 	2. Perform linear split
 * 	3. Reassign branches to nodes based on split
 */
void split_node_ls(struct Node *n, struct Branch *b, struct Node **nn)
{
	register struct PartitionVars *p;
	register int level;
	//Load BUFFER_SIZE+1 branches into buffer and re-initialize the old node.
	level = n->level;
	getSplitNodeBranches(n, b);
	p = &Partitions[0];
	/* Note: can't use MINFILL(n) below since n was cleared by GetBranches() */
	split_starter(p, BRANCH_FACTOR/2);
	/* put branches from buffer in 2 nodes according to chosen partition */
	*nn = newNode();
	(*nn)->level = n->level = level;
	finishPartition(n, *nn, p);
}
