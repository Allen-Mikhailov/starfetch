#ifndef VECTOR_H
#define VECTOR_H

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


typedef struct Vector2 {
	float x;
	float y;
} v2;

float pointD2(v2 *p1, v2 *p2)
{
	return (p1->x - p2->x) * (p1->x - p2->x) 
		+  (p1->y - p2->y) * (p1->y - p2->y);
}

void rotateVector2(v2 *v, float theta)
{
	float x = v->x;
	float y = v->y;
	v->x = x * cos(theta) - y * sin(theta);
	v->y = x * sin(theta) + y * cos(theta);
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
