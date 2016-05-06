#ifndef _INDEX_
#define _INDEX_

#define BRANCH_FACTOR 25

struct Rectangle
{
	float boundary[4];	//xmin, ymin, xmax, ymax
};
struct Branch
{
	struct Rectangle rect;
	struct Node *child;
};
struct Node
{
	int count;
	int level;
	struct Branch branch [BRANCH_FACTOR];
};
struct ListNode
{
	struct ListNode *next;
	struct Node *node;
};

typedef int (*UserDefFunction)(int id, void* arg);
extern struct Rectangle combineRect(struct Rectangle *, struct Rectangle *);
extern void init_Node(struct Node*);
extern struct Node* newNode();
extern void init_Rect(struct Rectangle*);
extern float rectSphericalVol(struct Rectangle*);
extern int addBranchToNode(struct Branch *, struct Node *, struct Node **);
extern void split_node_ls(struct Node *, struct Branch *, struct Node **);
extern struct Rectangle findCoveringRect(struct Node *);
extern int pickBranch(struct Rectangle *, struct Node *);
extern void disConnectBranch(struct Node *, int);
extern int checkIfOverlaps(struct Rectangle *, struct Rectangle *);
extern int isContained(struct Rectangle *, struct Rectangle *);
extern void freeNode(struct Node *p);
extern int insertRectangle(struct Rectangle *, int, struct Node **, int);
extern int intersection_Search(struct Node *, struct Rectangle *, UserDefFunction, void*);
extern struct Node* newRTreeIndex();
extern int containment_Search(struct Node *, struct Rectangle *, UserDefFunction, void*);
extern void printNodes(struct Node *currNode, int depth);
extern void print_Rectangle(struct Rectangle *, int);
extern void provideTabSpace(int);
extern void RTreePrintBranch(struct Branch *, int);
#endif /* _INDEX_ */
