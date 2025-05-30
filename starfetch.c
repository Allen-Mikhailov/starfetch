#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include "ascii.h"
#include "ascii_lights.h"
#include "ascii_rain.h"
#include <time.h>

void sleep_ms(int milliseconds)
{
    // Convert milliseconds to microseconds
    usleep(milliseconds * 1000);
}

char *scan_piped()
{
	int buffer_size = 8;
	int data_length = 0;

	char *buffer = (char*) malloc(sizeof(char) * buffer_size);

	// Reading one byte at a time
	char c[2];
	while (1)
	{

		fgets(c, 2, stdin);

		if (data_length == buffer_size)
		{
			buffer_size *= 2;
			buffer = realloc(buffer, buffer_size); 
		}

		buffer[data_length] = *c;
		data_length++;

		if (*c == '\0')
			break;

		*c = 0;
	}

	return buffer;
}

char *printAndReturnNull(char* str)
{
	printf("%s", str);
	return NULL;
}

char *clean_neopixel()
{
	char *neofetch_string;
	char *ascii_start_str = "[1m";
	char *ascii_end_str = "0m";

	neofetch_string = scan_piped();

	char *ascii_start = strstr(neofetch_string, ascii_start_str);
	char *ascii_end = strstr(ascii_start, ascii_end_str);

	if (ascii_start == NULL)
		return printAndReturnNull("ASCII Start is NULL\n");

	if (ascii_end == NULL)
		return printAndReturnNull("ASCII End is NULL\n");

	if (ascii_start > ascii_end)
		return printAndReturnNull("Start > end for some reason\n");

	ascii_start += 3;
	ascii_end -= 2;

	while (ascii_start != ascii_end)
	{
		*ascii_start = ' ';
		ascii_start++;
	}

	return neofetch_string;
}

C3 ambient_light = {35, 38, 39};


int main() {
	time_t start_time = time(NULL);
	time_t last_time = start_time;
	int fps;

	srand(start_time);

	char *neofetch_string = clean_neopixel();
	
	
	printf("\n");

	printf("%s\n", neofetch_string);


	AsciiBuffer *buffer = createAsciiBuffer(42, 21);
	ALBuffer *light_buffer = attachALBuffer(buffer);
	ARState *rain_state = attachARState(buffer, 50);

	Ellipse ellipse;
	ellipse.x = 20;
	ellipse.y = 20;
	ellipse.a = 10; 
	ellipse.b = 10;
	ellipse.theta = -0.63;

	Ellipse ellipse2;
	ellipse2.x = 20;
	ellipse2.y = 20;
	ellipse2.a = 4; 
	ellipse2.b = 2;
	ellipse2.theta = -0.63;

	APLight ap_light;
	ap_light.point.x = 20;
	ap_light.point.y = 10;
	ap_light.color.r = 253;
	ap_light.color.g = 200;
	ap_light.color.b = 68;
	ap_light.strength = 20;


	int frame = 0;
	int fps_frame = 0;

	while (1)
	{
		// Adjusting State
		ellipse2.theta = 0.63 + 0.5 * sin(frame * 0.02);


		

		// Drawing
		printf("\033[H");	

		clearAsciiBuffer(buffer);
		clearLights(light_buffer);

		tickRain(rain_state);

		drawRain(rain_state, buffer);

		ellipse.theta = 0.63;
		drawEllipse(buffer, &ellipse, 2);

		//ellipse.y = 20 + 1 * sin(frame * 0.02);
		drawEllipse(buffer, &ellipse2, 1);

		ap_light.point.y = 10 + 4 * sin(frame * 0.03);

		applyAmbientLight(light_buffer, &ambient_light);
		applyPointLight(light_buffer, &ap_light);

		
		printColoredBuffer(light_buffer);

		frame++;
		fps_frame++;

		if (time(NULL) != last_time)
		{
			last_time = time(NULL);
			fps = fps_frame;
			fps_frame = 0;	
		}

		printf("\033[H");	
		printf("%d \n", fps );


		
		sleep_ms(20);
	}





	return 0;
}
