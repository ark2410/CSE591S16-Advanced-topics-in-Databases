#include <stdio.h>
#include "Index.h"
#include <math.h>

/*
 * Initializes the rectangle and assigns all the coordinates as 0
 */
void init_Rect(struct Rectangle* newRectangle)
{
	register struct Rectangle* rectangleR = newRectangle;
	for (int i=0;i<4;i++)
	{
		rectangleR->boundary[i]=(float)0;
	}
}
/*
 * Computes minimum value between two double variables
 */
float computeMinimum(double p1, double p2)
{
	return p1<p2?p1:p2;
}
/*
 * Computes maximum value between two double variables
 */
float computeMaximum(double p1, double p2)
{
	return p1>p2?p1:p2;
}
/*
 * Constructs a rectangle that can contain the passed two rectangles.
 */
struct Rectangle combineRect(struct Rectangle *R1, struct Rectangle *R2)
{
	register struct Rectangle *r1 = R1, *r2=R2;
	struct Rectangle new_rect;
	new_rect.boundary[0] = computeMinimum(r1->boundary[0],r2->boundary[0]);
	new_rect.boundary[1] = computeMinimum(r1->boundary[1],r2->boundary[1]);
	new_rect.boundary[2] = computeMaximum(r1->boundary[2],r2->boundary[2]);
	new_rect.boundary[3] = computeMaximum(r1->boundary[3],r2->boundary[3]);
	return new_rect;
}
/*
 * Returns 1 if the two rectangles overlap.
 * Two rectangles do not overlap under the following conditions
 * 	1) One rectangle is on the left side of the left edge of other rectangle
 * 	2) One rectangle is above top edge of other rectangle
 * Return 1 if either of the above conditions fail.
 */
int checkIfOverlaps(struct Rectangle *R1, struct Rectangle *R2)
{
	register struct Rectangle *r1 = R1, *r2 = R2;
	if(r1->boundary[0]>r2->boundary[2] || r2->boundary[0]>r1->boundary[2])
		return 0;
	if(r1->boundary[1]>r2->boundary[3] || r2->boundary[1]>r1->boundary[3])
		return 0;
	return 1;
}
/*
 * Returns 1 if the index_rect is contained within window
 */
int isContained(struct Rectangle *index_rect, struct Rectangle *window)
{
	register struct Rectangle *iR = index_rect, *w = window;
	register int isXContained, isYContained;
	isXContained = iR->boundary[0] >= w->boundary[0] && iR->boundary[2]<=w->boundary[2];
	isYContained = iR->boundary[1] >= w->boundary[1] && iR->boundary[3]<=w->boundary[3];
	return isXContained && isYContained;
}
/*
 * Rectangle spherical volume is computed instead of area to remove zero width/length anomaly.
 * Spherical volume [for 2 dimensions] = [pow(((x2-x1)/2),2) + pow(((y2-y1)/2),2)]*UnitSphericalVol(2)
 * UnitSpherical(2) = 3.141593 [Precomputed]
 */
float rectSphericalVol(struct Rectangle *input_Rect)
{
	register struct Rectangle *iR = input_Rect;
	register double sumOfSquares=0;
	double half_len = iR->boundary[2]-iR->boundary[0];
	sumOfSquares += half_len*half_len;
	half_len = iR->boundary[3]-iR->boundary[1];
	sumOfSquares += half_len*half_len;
	return (float)(sumOfSquares*3.141593);
}
void print_Rectangle(struct Rectangle *index_Rect, int depth)
{
	register struct Rectangle *rR = index_Rect;
	provideTabSpace(depth);
	printf("rect:\n");
	provideTabSpace(depth+1);
	printf("%f\t%f\n", rR->boundary[0], rR->boundary[2]);
	provideTabSpace(depth+1);
	printf("%f\t%f\n", rR->boundary[1], rR->boundary[3]);
}
