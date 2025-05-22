#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>

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

char *clean_neopixel()
{
	char *neofetch_string;
	char *ascii_start_str = "[1m";
	char *ascii_end_str = "0m";

	neofetch_string = scan_piped();

	char *ascii_start = strstr(neofetch_string, ascii_start_str);
	char *ascii_end = strstr(ascii_start, ascii_end_str);

	if (ascii_start == NULL)
		return printf("ASCII Start is NULL\n");

	if (ascii_end == NULL)
		return printf("ASCII End is NULL\n");

	if (ascii_start > ascii_end)
		return printf("Start > end for some reason\n");

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

	printf("%s\n", neofetch_string);

	

	return 0;
}
