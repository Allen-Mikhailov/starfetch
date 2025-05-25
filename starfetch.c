#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include "ascii.h"

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


int main() {

	char *neofetch_string = clean_neopixel();
	
	
	printf("\n");

	printf("%s\n", neofetch_string);


	AsciiBuffer *buffer = createAsciiBuffer(42, 21);

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

	Ellipse ellipse3;
	ellipse3.x = 20;
	ellipse3.y = 20;
	ellipse3.a = 20; 
	ellipse3.b = 20;
	ellipse3.theta = -0.63;


	int frame = 0;


	while (1)
	{
		// Adjusting State
		ellipse2.theta = 0.63 + 0.3 * sin(frame * 0.02);


		// Drawing
		printf("\033[H");	

		clearAsciiBuffer(buffer);

		ellipse.theta = 0.63;
		drawEllipse(buffer, &ellipse, 2);


		drawEllipse(buffer, &ellipse2, 1);

		
		printAsciiBuffer(buffer);

		frame++;
		
		sleep_ms(50);
	}





	return 0;
}
