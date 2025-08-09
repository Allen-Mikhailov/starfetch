#ifndef ASCII_RAIN_H
#define ASCII_RAIN_H

#include "ascii.h"


typedef struct AsciiRainDrop {
	int x;
	int y;

} ARainDrop;

typedef struct AsciiRainState {
	int width;
	int height;
	float width_ratio;

	ARainDrop *rain_drops;
	int rain_drop_count;
} ARState;

ARState *attachARState(AsciiBuffer *buffer, int rain_drops)
{
	ARState *state = malloc(sizeof(ARState)); 

	state->width = buffer->width;
	state->height = buffer->height;

	state->width_ratio = (float) buffer->width / (buffer->width + buffer->height);

	state->rain_drop_count = rain_drops;
	state->rain_drops = (ARainDrop*) malloc(sizeof(ARainDrop) * rain_drops);
}

void tickRain(ARState *state)
{
	for (int i = 0; i < state->rain_drop_count; i++)
	{
		ARainDrop *drop = &state->rain_drops[i];
		drop->x -= 1;
		drop->y += 1;

		// Checking if the drop is out of boudns
		if (drop->x < 0 || drop->y == state->height)
		{
			float a = (float) (rand()%1000) * (0.001);
			
			if (state->width_ratio > a) {
				// On the top
				drop->x = (a / state->width_ratio) * state->width;
				drop->y = 0;
			} else {
				// On the side
				drop->x = state->width - 1;
				drop->y = (a - state->width_ratio) / (1-state->width_ratio) * state->height;
			}
		}
	}

}

void drawRain(ARState *state, AsciiBuffer *buffer)
{
	for (int i = 0; i < state->rain_drop_count; i++)
	{
		ARainDrop *drop = &state->rain_drops[i];
		buffer->buffer[drop->x + drop->y * buffer->width] = '/';
	}
}



#endif // !ASCII_RAIN_H
