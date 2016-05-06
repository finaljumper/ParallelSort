// ParallelSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstring>
#include <ppl.h>

double range(char code)
{
	if ((code >= 32 && code <= 47) || (code >= 58 && code <= 64) || (code >= 123 && code <= 126)
		|| (code >= 91 && code <= 96) || code == 13)
	{
		return code;
	}
	if (code >= 48 && code <= 57)
	{
		return code + 100;
	}
	if (code >= 65 && code <= 122)
	{
		if (code >= 97)
		{
			return code + 1500;
		}
		return code + 2000;
	}
	if (code <= -65 && code >= -128)
	{
		if (code >= -128 && code <= -113)
		{
			return (code + 1216);
		}
		if ((code >= -80 && code <= -65))
		{
			return (code + 1152);
		}
		return (code + 1500);
	}
	if (code == -47 || code == -48)
	{
		return 1040;
	}
	return code;
}
//À = -48 -112
//Á = -48 -111 
//ß = -48 -81
//î = -48 -66 
//ï = -48 -65 
//ð = -47 -128 
//ÿ = -47 -113
int compare(const char* s1, const char* s2, int nesting)
{
	if (!strcmp(s1, s2))
	{
		return -1;
	}
	if (strlen(s1) == nesting + 1)
	{
		return 1;
	}
	if (strlen(s2) == nesting + 1)
	{
		return -1;
	}
	register float str1 = range(s1[nesting]);
	register float str2 = range(s2[nesting]);
	if (str1 < str2)
	{
		return 1;
	}
	if (str1 > str2)
	{
		return -1;
	}
	else
	{
		return 0;
	}
	return -1;
}

bool comparator(const char* s1, const char* s2)
{
	register char c1 = s1[0];
	register char c2 = s2[0];
	if (range(c1) < range(c2))
	{
		if ((c1 == -47 || c1 == -48) && (c2 == -47 || c2 == -48))
		{
			int nesting = 1;
			while (!0)
			{
				int comp = compare(s1, s2, nesting);
				if (comp == 1)
					return true;
				else if (comp == -1)
					return false;
				nesting++;
			}
		}
		return true;
	}
	if (range(c1) > range(c2))
	{
		if ((c1 == -47 || c1 == -48) && (c2 == -47 || c2 == -48))
		{
			int nesting = 1;
			while (!0)
			{
				int comp = compare(s1, s2, nesting);
				if (comp == 1)
					return true;
				else if (comp == -1)
					return false;
				nesting++;
			}
		}
		return false;
	}
	else
	{
		int nesting = 1;
		while (!0)
		{
			int comp = compare(s1, s2, nesting);
			if (comp == 1)
				return true;
			else if (comp == -1)
				return false;
			nesting++;
		}
	}
	return false;
}

int main(int argc, char *argv[])
{
	char * input;
	char * output;
	int count = 1;
	char * buffer;
	char ** lines = 0;
	long lSize;
	long prevPos = 0;
	long currentLine = 0;
	long i;
	setlocale(LC_ALL, "Russian");
	if (argc == 3)
	{
		input = argv[1];
		output = argv[2];
	}
	else if (argc == 2 && argv[1] == "help")
	{
		std::cout << "Use first argument as input file and second one as output file." << std::endl;
		system("pause");
		exit(0);
	}
	else
	{
		std::cout << "Use help for help." << std::endl;
		system("pause");
		exit(0);
	}
	FILE * ptrFile = fopen(input, "rb");
	fseek(ptrFile, 0, SEEK_END);
	lSize = ftell(ptrFile);
	rewind(ptrFile);
	buffer = (char*)malloc(lSize);
	size_t result = fread(buffer, 1, lSize, ptrFile);
	for (i = 0; i < lSize; i++) {
		if (buffer[i] == '\n') {
			count++;
		}
	}
	lines = (char **)malloc(count * sizeof(char*));
	for (i = 0; i < lSize; i++)
	{
		if (buffer[i] == '\n' || i == lSize - 1) {
			lines[currentLine] = (char*)malloc(i - prevPos + 1);
			if (i == lSize - 1)
			{
				memcpy(lines[currentLine], &buffer[prevPos], i - prevPos + 1);
				if (lines[currentLine][i - prevPos] != 10)
				{
					lines[currentLine][i - prevPos + 1] = 10;
					lines[currentLine][i - prevPos + 2] = 0;
				}
				else
				{
					lines[currentLine][i - prevPos + 1] = 0;
				}
				currentLine++;
				break;
			}
			else
			{
				memcpy(lines[currentLine], &buffer[prevPos], i - prevPos);
			}
			lines[currentLine][i - prevPos] = 0;
			prevPos = i + 1;
			currentLine++;
		}
	}
	fclose(ptrFile);
	free(buffer);
	concurrency::parallel_buffered_sort(&lines[0], &lines[currentLine], comparator);	
	FILE * out = fopen(output, "wb");
	for (i = 0; i < currentLine; i++)
	{
		fwrite(lines[i], 1, strlen(lines[i]), out);
	}	
	fclose(out);
	free(lines);
	return 0;
}