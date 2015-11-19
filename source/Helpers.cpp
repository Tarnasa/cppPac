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

int random_int(std::mt19937& random, int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(random);
}

bool chance(std::mt19937& random, double chance)
{
	return std::uniform_real_distribution<double>(0, 1)(random) < chance;
}

std::string replace(const std::string& s, const std::string& search, const std::string& replacement)
{
	std::string n;
	int match_start = 0;
	int match_length = 0;
	for (int i = 0; i < s.size(); ++i)
	{
		if (s[i] == search[match_length])
		{
			if (match_length == 0) match_start = i;
			match_length += 1;
			if (match_length == search.size())
			{
				n += replacement;
				match_length = 0;
			}
		}
		else
		{
			n += s.substr(i - match_length, match_length);
			match_length = 0;
			if (s[i] == search[match_length])
			{
				if (match_length == 0) match_start = i;
				match_length += 1;
			}
			else
			{
				n += s[i];
			}
		}
	}
	n += s.substr(s.size() - match_length);
	return n;
}
