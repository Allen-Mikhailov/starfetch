#ifndef VECTOR_H
#define VECTOR_H

#include "table_sin.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


typedef struct Vector2 {
	float x;
	float y;
} v2;

static inline v2 cloneV2(v2 *p)
{
	v2 p2 = {p->x, p->y}; 
	return p2;
}

float pointD2(v2 *p1, v2 *p2)
{
	return (p1->x - p2->x) * (p1->x - p2->x) 
		+  (p1->y - p2->y) * (p1->y - p2->y);
}

static inline void rotateVector2(v2 *v, float theta)
{
	// printf("%f\n", theta);
	theta = fmodf(theta + M_PI*2, M_PI*2);
	float x = v->x;
	v->x = x * t_cos(theta) - v->y * t_sin(theta);
	v->y = x * t_sin(theta) + v->y * t_cos(theta);
}

static inline void rotateVector2O(v2 *v, float theta, float st, float ct)
{
	float x = v->x;
	v->x = x * ct - v->y * st;
	v->y = x * st + v->y * ct;
}

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

#endif
