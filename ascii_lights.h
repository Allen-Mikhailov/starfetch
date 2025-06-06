#ifndef ASCII_LIGHTS_H
#define ASCII_LIGHTS_H

#include "ascii.h"

typedef struct Color3 {
	unsigned short r;
	unsigned short g;
	unsigned short b;
} C3;

typedef struct AsciiLightBuffer {
	AsciiBuffer *ascii_buffer;
	C3 *colors;

	int width;
	int height;
	
} ALBuffer;

typedef struct AsciiPointLight {
	v2 point;

	C3 color;
	float strength;
} APLight;

ALBuffer *attachALBuffer(AsciiBuffer *buffer)
{
	ALBuffer *result = malloc(sizeof(ALBuffer));

	result->ascii_buffer = buffer;
	result->width = buffer->width;
	result->height = buffer->height;

	result->colors = malloc(sizeof(C3) * buffer->width * buffer->height);

	return result;
}

void applyAmbientLight(ALBuffer *buffer, C3 *light) 
{
	for (int x = 0; x < buffer->width; x++)
	{
		for (int y = 0; y < buffer->height; y++)
		{
			C3 *color = &buffer->colors[x + y * buffer->width];
			color->r += light->r;
			color->g += light->g;
			color->b += light->b;
		}
	}
	
}



void applyPointLight(ALBuffer *buffer, APLight *light)
{
	float b = 1 / light->strength;

	rect ascii_bounds; 
	getAsciiBounds(buffer->ascii_buffer, &ascii_bounds);

	rect light_bounds;
	getCircleBoundsRect(light->strength, light->point, ascii_bounds, &light_bounds);

	v2 c_point; // cell point
	for (int x = light_bounds.left; x < light_bounds.right+1; x++)
	{
		c_point.x = x + 0.5;
		for (int y = light_bounds.top; y < light_bounds.bottom+1; y++)
		{
			c_point.y = 2*y + 1;
			float distance = sqrt(pointD2(&c_point, &(light->point)));
			//printf("Distance %f\n", distance);

			float brightness = MAX(1.0 - (b * distance), 0);

			C3 *color = &buffer->colors[x + buffer->width * y];
			color->r += light->color.r * brightness;
			color->g += (unsigned char)((float) light->color.g * brightness);
			color->b += (unsigned char)((float) light->color.b * brightness);
		}
	}
}

void clearLights(ALBuffer *buffer)
{
	C3 *head = buffer->colors;
	int length = buffer->width*buffer->height;
	for (int i = 0; i < length; i++)
	{
		head->r = 0;
		head->g = 0;
		head->b = 0;
		head++;
	}
}

void writeNumCharToStr(char* str, unsigned char val)
{
	*str = '0' + (val / 100);
	str++;
	*str = '0' + ((val%100) / 10);
	str++;
	*str = '0' + (val%10);
}

const char COLOR_STR[] = "\033[38;2;000;000;000m";
const int COLOR_STR_SIZE = sizeof(COLOR_STR) - 1;

void printColoredBuffer(ALBuffer *buffer)
{
	// size of string is 20 * character count + height (for \n) + 1 (\0)
	int char_count = buffer->width * buffer->height;
	int malloc_size = sizeof(char) * (COLOR_STR_SIZE +1)* char_count + buffer->height + 1 + COLOR_STR_SIZE;
	char *str = malloc(malloc_size);

	char *str_head = str;

	AsciiBuffer *ascii_buffer = buffer->ascii_buffer;
	char *character_buffer_head = ascii_buffer->buffer;

	for (int y = 0; y < buffer->height; y++)
	{
		for (int x = 0; x < buffer->width; x++)
		{
			// getting the color;
			C3 *color = &buffer->colors[x + y * buffer->width];
			
			// Copying the color string in
			memcpy(str_head, COLOR_STR, COLOR_STR_SIZE);

			writeNumCharToStr(str_head+7,  MIN(color->r, 255));
			writeNumCharToStr(str_head+11, MIN(color->g, 255));
			writeNumCharToStr(str_head+15, MIN(color->b, 255));

			str_head += COLOR_STR_SIZE;

			// Writing the character
			*str_head = *character_buffer_head;
			str_head++;
			character_buffer_head++;
		}

		*str_head = '\n';
		str_head++;

	}

	// Writing White
	memcpy(str_head, COLOR_STR, COLOR_STR_SIZE);
	writeNumCharToStr(str_head+7,  255);
	writeNumCharToStr(str_head+11, 255);
	writeNumCharToStr(str_head+15, 255);
	str_head += COLOR_STR_SIZE;

	*str_head = '\0';

	printf("%s", str);
	free(str);

}

#endif // ASCII_LIGHTS_H
