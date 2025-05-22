#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <math.h>

#define USE_STRUCT(ptr, name) struct typeof(*(ptr)) name = *(ptr)

typedef struct EllipseStruct {
	float x;
	float y;
	float theta;
	float a;
	float b;
} Ellipse;


typedef struct QuadraticResult {
	char count;
	float v1;
	float v2;
} QResult;

// Point Vector Line
// Vector does not have to be normalized
typedef struct PointVectorLine {
	float px;
	float py;
	float vx;
	float vy;
} PVLine;

void solveQuadratic(float a, float b, float c, QResult *result)
{
	float d = b*b - 4 * a * c;
	if (d > 0) {
		result->count = 2;
		result->v1 = (-b + sqrt(d)) / (2*a);
		result->v2 = (-b - sqrt(d)) / (2*a);
	} else if (d == 0) {
		result->count = 1;
		result->v1 = (-b) / (2*a);
	} else {
		result->count = 0;
	}
}

typedef struct Vector2 {
	float x;
	float y;
} v2;

void rotateVector2(v2 *v, float theta)
{
	float x = v->x;
	float y = v->y;
	v->x = x * cos(theta) - y * sin(theta);
	v->y = x * sin(theta) + y * cos(theta);
}

// Returns the time that the line will intersect with the ellipse
void ellipseIntersection(Ellipse *ellipseS, PVLine *raw_line, QResult *result)
{
	USE_STRUCT(ellipseS, ellipse);

	// Rotating the line
	v2 line_point = {raw_line->px, raw_line->py};
	v2 line_vector = {raw_line->vx, raw_line->vy};

	rotateVector2(*line_point, ellipse.theta);
	rotateVector2(*line_vector, ellipse.theta);

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

v2 getPointOnEllipse(Ellipse *ellipse, float t)
{
	v2 p = {cos(t) * ellipse->a, sin(t) * ellipse->b};
	rotateVector2(&p, ellipse->theta);
	p.x += ellipse->x;
	p.y += ellipse->y;
	return p;
}

#endif
