#include "Index.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>

/*struct Rectangle inputRectangles[] =
{
	{0, 0, 2, 2}, // xmin, ymin, xmax, ymax (for 2 dimensional RTree)
	{5, 5, 7, 7},
	{8, 5, 9, 6},
	{7, 1, 9, 2},
	{6, 4, 10, 6},
};*/

int k = 100;
int current_count = 0;

//int noOfRectangles = sizeof(inputRectangles) / sizeof(inputRectangles[0]);
struct Rectangle searchRect = {
	{-104.046822f, 45.559261, -101.023819, 46.660042}, // search will find above rects that this one overlaps
};

int customPrint(int id, void* arg)
{
	// Note: -1 to make up for the +1 when data was inserted
	//printf("Id of rectangle satisfying query %d\n", id);
	return 1; // keep going
}
int customPrint1(int id, void* arg)
{
	current_count++;
	printf("%d ",id);
	if(current_count==k)
		return 0;
	return 1;
}

void readFile(struct Node* root)
{
	int int_result=0,cont_result = 0,i;
	struct Rectangle inputData [99465];
	FILE *fp = fopen("/home/anandh/Documents/Advanced Databases/testData/100krect.txt","r");
	int count = 0;
	int temp=0;
	while(fscanf(fp,"%d,%f,%f,%f,%f",&temp,&inputData[count].boundary[0],&inputData[count].boundary[1],
			&inputData[count].boundary[2],&inputData[count].boundary[3])!=EOF)
	{
		/*printf("%f,%f,%f,%f\n",inputData[count].boundary[0],inputData[count].boundary[1],inputData[count].boundary[2],
				inputData[count].boundary[3]);*/
		count++;
	}
	printf("Count: %d\n",count);

	for(i=0;i<count;i++)
	{
		insertRectangle(&inputData[i],i+1,&root,0);
	}
	//printNodes(root,0);
	//int_result = intersection_Search(root, &searchRect, customPrint, 0);
	printf("%d\n",root->level);
	//printf("------End of Intersection-----\n");
	clock_t cstart = clock();
		clock_t cend = 0;
	cont_result = containment_Search(root, &searchRect, customPrint, 0);
	printf("Intersect Result Count: %d\n",int_result);
	printf("Containment Result Count: %d\n",cont_result);
	cend = clock();
	printf ("%.6f cpu sec\n", ((double)cend - (double)cstart)* 1.0e-6);
}

/*int main()
{
	struct Node* root = newRTreeIndex();
	int i, intersectResult,containmentResult;
	printf("Number of Rectangles = %d\n", noOfRectangles);
	for(i=0; i<noOfRectangles; i++)
		insertRectangle(&inputRectangles[i], i+1, &root, 0); // i+1 is rect ID. Note: root can change
	printNodes(root,0);
	intersectResult = intersection_Search(root, &searchRect, customPrint, 0);
	printf("Search resulted in %d hits\n", intersectResult);
	printf("Root level %d\n",root->level);
	containmentResult = containment_Search(root, &searchRect, customPrint, 0);
	printf("Search resulted in %d hits1\n", containmentResult);
	return 0;
}*/

int main()
{
	struct Node* root = newRTreeIndex();
	readFile(root);
	return 0;
}
