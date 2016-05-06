#include "Index.h"

/*
 * All branches of the node considered for split.
 */
struct Branch BranchBuf[BRANCH_FACTOR+1];
/*
 * Total number of branches considered for the partition
 */
int BranchCount;
/*
 * Covering rectangle for all the nodes that are considered during split
 */
struct Rectangle CoverSplit;

/*
 * Temporary structure used in linear split algorithm of a node
 */
struct PartitionVars
{
	/*
	 * Each variable in the array takes value 0 or 1
	 * 0 is node1 and 1 is node 2
	 */
	int assignedTo[BRANCH_FACTOR+1];
	/*
	 * total -> The total number of nodes (usually BRANCH_COUNT+1) considered during partition
	 * minfill -> The minimum number of branches allowed in a node.
	 */
	int total, minfill;
	/*
	 * Flag to check if a particular branch has been assigned to either of the nodes
	 */
	int isAssigned[BRANCH_FACTOR+1];
	/*
	 * count[0] -> Count in first split node
	 * count[1] -> Count in new split node
	 */
	int count[2];
	/*
	 * Covering rectangle for both the nodes
	 */
	struct Rectangle cover[2];
	/*
	 * Area for both the nodes
	 */
	float area[2];
} Partitions[1];
