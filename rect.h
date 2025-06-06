#ifndef RECT_H
#define RECT_H

#include "vector.h"

typedef struct FRectStruct {
	float left;
	float right;
	float top;
	float bottom;
} frect;

typedef struct RectStruct {
	int left; 
	int right;
	int top;
	int bottom;
} rect;

void *getCircleBoundsRect(float r, v2 pos, rect bounds, rect* out)
{
	out->left = MAX((int) (pos.x - r), bounds.left);
	out->right = MIN((int) (pos.x + r + 0.9999), bounds.right);
	out->top = MAX((int) (pos.x - r), bounds.top);
	out->bottom = MIN((int) (pos.y + r + 0.9999), bounds.bottom);
}

void printRect(rect r)
{

	printf("left %d, right %d, top %d, bottom %d\n", r.left, r.right, r.top, r.bottom);
}


#endif // !RECT_H
