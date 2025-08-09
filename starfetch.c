#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include "ascii.h"
#include "ascii_lights.h"
#include "ascii_rain.h"
#include <time.h>
#include "table_sin.h"

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

#define EXPECTED_FASTFETCH_SIZE 8000

char *readFastFetch()
{
	FILE *fp;
	char *output = malloc(EXPECTED_FASTFETCH_SIZE);
	char section[1035];

	/* Open the command for reading. */
	fp = popen("fastfetch", "r");
	if (fp == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}

	/* Read the output a line at a time - output it. */
	while (fgets(section, sizeof(section), fp) != NULL) {
		// printf("%s", section);
		strcat(output, section);
	}

	/* close */
	pclose(fp);

	return output;
}

char *printAndReturnNull(char* str)
{
	printf("%s", str);
	return NULL;
}

char *clean_neopixel()
{
	// system("fastfetch");

	char *neofetch_string;
	char *ascii_end_str = "[1G";

	neofetch_string = readFastFetch();

	char *ascii_end = strstr(neofetch_string, ascii_end_str);
	char *ascii_start;

	if (ascii_end == NULL)
		return printAndReturnNull("ASCII End is NULL\n");

	ascii_start = neofetch_string; 
	ascii_end -= 4;

	while (ascii_start != ascii_end)
	{
		*ascii_start = ' ';
		ascii_start++;
	}

	return neofetch_string;
}

C3 ambient_light = {35, 38, 39};

#define BUFFER_WIDTH 42
#define BUFFER_HEIGHT 21


int main() {
	time_t start_time = time(NULL);
	time_t last_time = start_time;
	int fps;

	init_sin_table();

	srand(start_time);

	char *neofetch_string = clean_neopixel();

	printf("\n");

	printf("%s\n", neofetch_string);


	AsciiBuffer *buffer = createAsciiBuffer(BUFFER_WIDTH, BUFFER_HEIGHT);
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
	ap_light.color.r = 219;
	ap_light.color.g = 110;
	ap_light.color.b = 179;
	ap_light.strength = 20;


	int frame = 0;
	int fps_frame = 0;

	const int fps_spacing = 4;

	char *fps_str = malloc(sizeof(char) * (BUFFER_HEIGHT + 2 + fps_spacing + 1));
	fps_str[0] = '%';
	fps_str[1] = 'd';

	for (int i = 0; i < fps_spacing; i++)
		fps_str[i+2] = ' ';

	for (int i = 0; i < BUFFER_HEIGHT; i++)
		fps_str[i+2+fps_spacing] = ' ';

	fps_str[2+fps_spacing+BUFFER_HEIGHT] = '\0';

	while (1)
	{
		// Adjusting State
		ellipse2.theta = 0.63 + 0.5 * sin(frame * 0.02);

		// Drawing

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

		frame++;
		fps_frame++;

		if (time(NULL) != last_time)
		{
			last_time = time(NULL);
			fps = fps_frame;
			fps_frame = 0;	
		}

		#define DISPLAY
		#ifdef DISPLAY
		
		printf("\033[H");	
		printColoredBuffer(light_buffer);

		printf("\033[H");	
		printf(fps_str, fps);

		#endif /* ifdef DISPLAY */	
		
		sleep_ms(30);
	}





	return 0;
}
