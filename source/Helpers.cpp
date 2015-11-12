#include "Helpers.h"

// Calculates the character length of an integer
int get_length_int(int n)
{
	char buffer[32];
	int length = sprintf(buffer, "%i", n);
	return length;
}

// Calculates the length formatted string
int get_printf_length(int max_length, const char* format, ...)
{
	// Weird c stuff ahead
	va_list args;
	char* buffer = new char[max_length];

	va_start(args, format);
	int length = snprintf(buffer, max_length, format, args);
	va_end(args);

	delete[] buffer;
	return length;
}
