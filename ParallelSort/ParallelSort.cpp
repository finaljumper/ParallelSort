// ParallelSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstring>
//#include <algorithm>
#include <time.h>
#include <ppl.h>

double range(char code)
{
	double SYMBOLS = 0.1;
	double DIGITS = 0.3;
	double CYRILLIC = 2;
	double SMALL_CYRILLIC = 0.7;
	double SMALL_LATIN = 200;
	double LATIN = 400;

	if ((code >= 32 && code <= 47) || (code >= 58 && code <= 64) || (code >= 123 && code <= 126)
		|| (code >= 91 && code <= 96) || code == 13)
	{
		return code*SYMBOLS;
	}
	if (code >= 48 && code <= 57)
	{
		return code*DIGITS;
	}
	if (code >= 65 && code <= 122)
	{
		if (code >= 97)
		{
			return code * SMALL_LATIN;
		}
		return code*LATIN;
	}
	if (code <= -65 && code >= -128)
	{
		if (code >= -128 && code <= -113)
		{
			return (code + 1216)*SMALL_CYRILLIC;
		}
		if ((code >= -80 && code <= -65))
		{
			return (code + 1152)*SMALL_CYRILLIC;
		}
		return (code + 1152)*CYRILLIC;
	}
	if (code == -47 || code == -48)
	{
		return 1040;
	}
	return code;
}
//А = -48 -112
//Б = -48 -111 
//Я = -48 -81
//о = -48 -66 
//п = -48 -65 
//р = -47 -128 
//я = -47 -113
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
	std::cout << "Reading file..." << std::endl;
	FILE * ptrFile = fopen(input, "rb");
	if (ptrFile == NULL)
	{
		std::cout << "File error.";
		system("pause");
		exit(1);
	}
	// определяем размер файла
	fseek(ptrFile, 0, SEEK_END);
	lSize = ftell(ptrFile);
	rewind(ptrFile);
	buffer = (char*)malloc(lSize);
	if (buffer == NULL)
	{
		std::cout << "Memory error.";
		system("pause");
		exit(2);
	}

	size_t result = fread(buffer, 1, lSize, ptrFile);
	if (result != lSize)
	{
		std::cout << "Reading error.";
		system("pause");
		exit(3);
	}
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
	clock_t sort_start = clock();
	std::cout << "Reading finished in: " << sort_start / (float)CLOCKS_PER_SEC << " sec" << std::endl;
	fclose(ptrFile);
	free(buffer);
	concurrency::parallel_buffered_sort(&lines[0], &lines[currentLine], comparator);
	//std::sort(&lines[0], &lines[currentLine], comparator);
	std::cout << "Sort completed. Time spent: " << (clock() - sort_start) / (float)CLOCKS_PER_SEC << " sec" << std::endl;
	FILE * out = fopen(output, "wb");
	for (i = 0; i < currentLine; i++)
	{
		fwrite(lines[i], 1, strlen(lines[i]), out);
	}
	std::cout << "Complete time: " << clock() / (float)CLOCKS_PER_SEC << " sec" << std::endl;
	fclose(out);
	free(lines);
	system("pause");
	return 0;
}