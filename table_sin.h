#ifndef TABLE_SIN_H
#define TABLE_SIN_H

#include <math.h>

#define TABLE_SIZE 6000

const float INDEX_TO_T = (M_PI * 2.0f / TABLE_SIZE);
const float T_TO_INDEX = (1/INDEX_TO_T);

float sin_table[TABLE_SIZE];
float cos_table[TABLE_SIZE];

void init_sin_table()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		float t = INDEX_TO_T * i;
		sin_table[i] = sin(t);
		cos_table[i] = cos(t);
	}
}

float t_cos(float t)
{
	return cos_table[(int) (t* T_TO_INDEX)];
}

float t_sin(float t)
{
	return sin_table[(int) (t* T_TO_INDEX)];
}

#endif
