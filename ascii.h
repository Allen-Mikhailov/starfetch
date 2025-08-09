#ifndef ASCII_H
#define ASCII_H

#include <stdlib.h>
#include <math.h>
#include "ellipse.h"
#include "rect.h"

#define PI 3.14159265358979323846

//char ASCII_GRADIENT[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

const char ASCII_GRADIENT[] = " .:-=+*#%@";
const int  ASCII_GRADIENT_COUNT = sizeof(ASCII_GRADIENT) -1;
#define ASCII_GRADINET_INVERTED

typedef struct AsciiBufferS {
	char* buffer;
	char *color_buffer;
	int width;
	int height;
} AsciiBuffer;

void clearAsciiBuffer(AsciiBuffer *buffer)
{
	for (int i = 0; i < buffer->width * buffer->height; i++)
		buffer->buffer[i] = ' ';
}

AsciiBuffer *createAsciiBuffer(int width, int height)
{
	AsciiBuffer *structure = (AsciiBuffer*) malloc(sizeof(AsciiBuffer));
	char* buffer = (char*) malloc(sizeof(char) * width * height);
	structure->buffer = buffer;
	structure->width = width;
	structure->height = height;

	clearAsciiBuffer(structure);

	return structure;
}

void getAsciiBounds(AsciiBuffer *buffer, rect *out)
{
	out->left = 0;
	out->top = 0;
	out->right = buffer->width-1;
	out->bottom = buffer->height-1;
}

char rangeToChar(float val, float min, float max)
{
	if (val < min)
		val = min;

	if (val >= max)
		val = max*0.999;

	float a = (val - min) / (max - min);

	// printf("a: %f\n", a);

#ifdef ASCII_GRADINET_INVERTED
	return ASCII_GRADIENT[(int) ((1-a)*ASCII_GRADIENT_COUNT)];
#elif 
	return ASCII_GRADIENT[(int) (a*ASCII_GRADIENT_COUNT)];
#endif
}


void freeAsciiBuffer(AsciiBuffer *buffer)
{
	free(buffer->buffer);
	free(buffer);
}

static inline int sameSign(float A, float B)
{
	return (A*B) >= 0;
}

// returns the time of the closest point on the ellipse
float closestPointToEllipse(v2 point, Ellipse *ellipse, float tol)
{
	float tol2 = tol*tol;

	// Rotating point to match ellipse rotation
	v2e r_point = *convertV2ToV2e(ellipse, &point);

	int above_ellipse   = r_point.y > 0;
	int rightof_ellipse = r_point.x > 0;

	if (r_point.x == 0 && r_point.y == 0) {
		if (ellipse->a > ellipse->b)
			return 0;
		else 
			return PI * 0.5;
	}

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

	// This is to prevent 0s from messing up things
	// const float POINT_SEARCH_EXPAND = 0.0;
	// t1 -= POINT_SEARCH_EXPAND;
	// t2 += POINT_SEARCH_EXPAND;

	d1 = distanceDerivative(ellipse, &r_point, t1);
	d2 = distanceDerivative(ellipse, &r_point, t2);

	// They must have opposite signs otherwise the algorithm does not work
	if (d1*d2 > 0)
	{
	// 	printf("Same sign failure %f %f\n", d1, d2);
		return -1;
	}

	while (fabsf(t1-t2) > tol)
	{
		t3 = (t1 + t2) * 0.5;
		d3 = distanceDerivative(ellipse, &r_point, t3);

		if (sameSign(d3,d2))
		{
			d2 = d3;
			t2 = t3;
		} else {
			d1 = d3;
			t1 = t3;
		}

		
	}

	return t1;
}

const float ellipse_tol = 0.03;
void drawEllipse(AsciiBuffer *buffer, Ellipse* ellipse, float width)
{
	v2 p;
	for (int x = 0; x < buffer->width; x++) {
		for (int y = 0; y < buffer->height; y++) {
			p.x = x + 0.5;
			p.y = (y + 0.5) * 2;

			// v2e r_point_e = convertV2ToV2e(ellipse, p);

			float point_time = fmodf(closestPointToEllipse(cloneV2(&p), ellipse, ellipse_tol), M_PI*2);

			if (point_time == -1)
			{
				// buffer->buffer[x + y*buffer->width] = '9';
				continue;	
			}

			v2 e_point = getPointOnEllipse(ellipse, point_time);
			float distance2 = pointD2(&e_point, &p);

			// printf("%d %d %f %f\n", x, y, point_time, distance2);

			if (distance2 > width*width)	
				continue;

			buffer->buffer[x + y*buffer->width] = rangeToChar(distance2, 0, width);
		}
	}
}

const char *ANGLE_ASCII = "|/'--'\\|";
const int ANGLE_ASCII_COUNT = 8;

void printAsciiBuffer(AsciiBuffer *buffer)
{
	int string_size = (buffer->width+1) * buffer->height + 1;
	char *str = malloc(sizeof(char) * string_size);

	char *str_head = str;
	char *buffer_head = buffer->buffer;


	for (int y = 0; y < buffer->height; y++)
	{
		for (int x = 0; x < buffer->width; x++)
		{
			
			*str_head = *buffer_head;
			if (*str_head == '\0')
				*str_head = 'X';
			buffer_head++;
			str_head++;
		}

		*str_head = '\n';
		str_head++;
	}

	*str_head = '\0';

	printf("%s", str);

}

#endif
