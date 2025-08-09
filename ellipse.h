#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <math.h>
#include "vector.h"
#include "table_sin.h"


typedef struct EllipseStruct {
	float x;
	float y;
	float theta;
	float a;
	float b;
} Ellipse;


typedef struct Vector2Ellipse {
	float x;
	float y;
} v2e;


// Returns the time that the line will intersect with the ellipse
void ellipseIntersection(Ellipse *ellipseS, PVLine *raw_line, QResult *result)
{
	Ellipse ellipse = *ellipseS;

	// Rotating the line
	v2 line_point = {raw_line->px, raw_line->py};
	v2 line_vector = {raw_line->vx, raw_line->vy};

	rotateVector2(&line_point, ellipse.theta);
	rotateVector2(&line_vector, ellipse.theta);

	PVLine line = {
		line_point.x,
		line_point.y,
		line_vector.x,
		line_vector.y
	};


	// Calculating fomr numbers to help use later
	float ea2 = 1.0 / (ellipse.a*ellipse.a); // inverse of Ellipse A^2
	float eb2 = 1.0 / (ellipse.b*ellipse.b); // inverse of Ellipse B^2


	// Calculating the a,b,c in the quadratic equation
	float a = line.vx*line.vx * ea2 + line.vy*line.vy * eb2;
	float b = 2*line.vx*line.px * ea2 + 2*line.vy*line.py*eb2;
	float c = line.px*line.px*ea2 + line.py*line.py*eb2 - 1;

	solveQuadratic(a, b, c, result);
}

static inline v2e getRelativePointOnEllipse(Ellipse *ellipse, float t)
{
	v2e result = {cos(t) * ellipse->a, sin(t) * ellipse->b};
	return result;
}

static inline v2 *convertV2eToV2(Ellipse *ellipse, v2e *p)
{
	rotateVector2((v2*) p, ellipse->theta);
	p->x += ellipse->x;
	p->y += ellipse->y;
	return (v2*) p;
}

static inline v2e *convertV2ToV2e(Ellipse *ellipse, v2 *p)
{
	p->x -= ellipse->x;
	p->y -= ellipse->y;
	rotateVector2(p, -ellipse->theta);
	return (v2e*) p;
}

static inline v2 getPointOnEllipse(Ellipse *ellipse, float t)
{
	v2 p = {cos(t) * ellipse->a, sin(t) * ellipse->b};
	convertV2eToV2(ellipse, (v2e*) &p);
	return p;
}

static inline float distanceDerivative(Ellipse *ellipse, v2e *point, float t)
{
	float s = t_sin(t);
	float c = t_cos(t);
	return 2 * (ellipse->a * c - point->x) * (-ellipse->a * s) + 2 * (ellipse->b * s - point->y) * ( ellipse->b * c);
}

#endif
