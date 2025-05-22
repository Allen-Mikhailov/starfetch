#ifndef ASCII_H
#define ASCII_H

#include <stdlib.h>
#include <math.h>
#include "ellipse.h"

#define PI 3.14159265358979323846

typedef struct AsciiBufferS {
	char* buffer;
	int width;
	int height;
} AsciiBuffer;


AsciiBuffer *createAsciiBuffer(int width, int height)
{
	AsciiBuffer *structure = (AsciiBuffer*) malloc(sizeof(AsciiBuffer));
	char* buffer = (char*) malloc(sizeof(char) * width * height);
	structure->buffer = buffer;
	structure->width = width;
	structure->height = height;

	return structure;
}

void clearAsciiBuffer(AsciiBuffer *buffer)
{
	for (int i = 0; i < buffer->width * buffer->height; i++)
		buffer->buffer[i] = ' ';
}

void freeAsciiBuffer(AsciiBuffer *buffer)
{
	free(buffer->buffer);
	free(buffer);
}

void drawEllipseAttemp1(AsciiBuffer *buffer, Ellipse* ellipse)
{
	// Calculating the intersection points on all the vertical and 
	// horizontal lines that make up the grid
	float *hIntersects = (float*) malloc(sizeof(float) * ( buffer->width + 1 ) *2);
	float *hIntersectsCount = (float*) malloc(sizeof(char) * (buffer->width + 1));

	float *vIntersects = (float*) malloc(sizeof(float) * ( buffer->height + 1 )*2);
	float *vIntersectsCount = (float*) malloc(sizeof(char) * (buffer->height + 1));

	PVLine line;
	QResult q_result;

	// Horizontal Lines
	line.px = 0;
	line.vx = 1;
	line.vy = 0;
	for (int i = 0; i < buffer->width+1; i++)
	{
		line.py = i;	
		ellipseIntersection(ellipse, &line, &q_result);
		hIntersectsCount[i] = q_result.count;

		// If count < 2 then these will be random values
		// but it does not matter because we will not check
		hIntersects[i*2+0] = q_result.v1;
		hIntersects[i*2+1] = q_result.v2;
	}

	// Vertical Lines
	line.py = 0;
	line.vx = 0;
	line.vy = 1;
	for (int i = 0; i < buffer->height+1; i++)
	{
		line.px = i;	
		ellipseIntersection(ellipse, &line, &q_result);
		vIntersectsCount[i] = q_result.count;

		// If count < 2 then these will be random values
		// but it does not matter because we will not check
		vIntersects[i*2+0] = q_result.v1;
		vIntersects[i*2+1] = q_result.v2;
	}

	for (int x = 0; x < buffer->width; x++) {
		for (int y = 0; y < buffer->height; y++) {
			// We need to check for the intersections of sides of the cell
			// top/bottom/left/right. The character is dependent on 
			// the sides in which the ellipse intersects 
			// dealing with corner intersections will be annoying
			
		}
	}
}

float pointD2(v2 *p1, v2 *p2)
{
	return (p1->x - p2->x) * (p1->x - p2->x) 
		+  (p1->y - p2->y) * (p1->y - p2->y);
}

float distanceDerivative(Ellipse *ellipse, float t)
{
	return 2 * (ellipse->a * cos(t) - ellipse->x) * (-ellipse->a * sin(t)) 
	     + 2 * (ellipse->b * sin(t) - ellipse->y) * ( ellipse->b * cos(t));
}

float closestPointToEllipse(v2 point, Ellipse *ellipse, float tol)
{
	float tol2 = tol*tol;

	// Rotating point to match ellipse rotation
	v2 r_point = {point.x-ellipse->x, point.y-ellipse->y};
	rotateVector2(&r_point, -ellipse->theta);

	bool above_ellipse   = r_point.y > ellipse->y;
	bool rightof_ellipse = r_point.x > ellipse->x;

	float t1;
	float t2;
	float t3;

	float d1;
	float d2;
	float d3;

	// Picking the starting quadrant
	if (above_ellipse)
	{
		if (rightof_ellipse)
		{
			// First Quadrant
			t1 = PI * 0;
			t2 = PI * 0.5;
		} else {
			// Second Quadrant
			t1 = PI * 0.5;
			t2 = PI * 1;
		}
	} else {
		if (!rightof_ellipse)
		{
			// Third Quadrant
			t1 = PI * 1;
			t2 = PI * 1.5;
		} else {
			// Fourth Quadrant
			t1 = PI * 1.5;
			t2 = PI * 2;
		}
	}

	d1 = distanceDerivative(ellipse, t1);
	d2 = distanceDerivative(ellipse, t2);

	while (pointD2(&p1, &p2) > tol2)
	{
		t3 = (t1 + t2) * 0.5;
		p3 = getPointOnEllipse(ellipse, t3);
		d3 = pointD2(&point, &p3);
	}
}

void drawEllipse(AsciiBuffer *buffer, Ellipse* ellipse)
{
	PVLine perp_line;
	for (int x = 0; x < buffer->width; x++) {
		for (int y = 0; y < buffer->height; y++) {
			perp_line.px = x + 0.5;
			perp_line.py = y + 0.5;

		}
	}
}

#endif
