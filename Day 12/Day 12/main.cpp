#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#define MAX_LINE_LENGTH 1000
#define PATTERN_LENGTH 5

using namespace std;

struct MapType
{
	char word[PATTERN_LENGTH + 1] = { 0 };
	char output;
};

ifstream fin("state.in");
ofstream fout("state.out");

const int pivotIndex = MAX_LINE_LENGTH / 2;

int potsCharsLength = 0;
char potsChars[MAX_LINE_LENGTH] = { NULL };
char secondaryBuffer[MAX_LINE_LENGTH] = { NULL };

map<int, vector<MapType> > patternsMap;

int GetKey(char* chars, int lgh)
{
	int result = 0;

	for (int i = 0; i < lgh; i++)
	{
		result += chars[i];
	}

	return result;
}

void Read()
{
	char line[MAX_LINE_LENGTH] = { 0 };
	while (fin.getline(line, MAX_LINE_LENGTH))
	{
		if (line[0] == 'i')
		{
			sscanf(line, "initial state: %s", potsChars + pivotIndex);
			
			potsCharsLength = strlen(potsChars + pivotIndex);

			for (int i = pivotIndex + potsCharsLength; i < MAX_LINE_LENGTH; i++)
			{
				potsChars[i] = '.';
			}

			potsChars[MAX_LINE_LENGTH - 1] = NULL;
		}

		if (line[0] == '.' || line[0] == '#')
		{
			MapType pattern = MapType();
			int key;

			sscanf(line, "%s => %c", pattern.word, &pattern.output);

			key = GetKey(pattern.word, PATTERN_LENGTH);

			if (patternsMap.find(key) == patternsMap.end())
			{
				patternsMap[key] = vector<MapType>();
			}

			patternsMap[key].push_back(pattern);
		}
	}
}

void Generation(char* activeBuffer, char* newBuffer)
{
	for (int i = 2; i < MAX_LINE_LENGTH - 2; i++)
	{
		// this can be optimized with a sum matrix that directly calculates the key
		char actualPattern[] = { activeBuffer[i - 2], activeBuffer[i - 1], activeBuffer[i], activeBuffer[i + 1], activeBuffer[i + 2], NULL };
		int key = GetKey(actualPattern, PATTERN_LENGTH);
		newBuffer[i] = '.';

		for (int j = 0; j < patternsMap[key].size(); j++)
		{
			if (strcmp(patternsMap[key][j].word, actualPattern) == 0)
			{
				newBuffer[i] = patternsMap[key][j].output;
			}
		}
	}
}

void Solve1()
{
	int sum = 0;
	char* activeBuffer = potsChars;
	char* newBuffer = secondaryBuffer;

	for (int i = 0; i < 20; i++)
	{
		Generation(activeBuffer, newBuffer);

		char* tmp = activeBuffer;
		activeBuffer = newBuffer;
		newBuffer = tmp;
	}

	for (int i = 0; i < MAX_LINE_LENGTH; i++)
	{
		if (activeBuffer[i] == '#')
		{
			sum += i - pivotIndex;
		}
	}

	fout << sum << endl;
}

int main(void)
{
	memset(secondaryBuffer, '.', sizeof(char) * MAX_LINE_LENGTH);
	memset(potsChars, '.', sizeof(char) * MAX_LINE_LENGTH);

	potsChars[MAX_LINE_LENGTH - 1] = NULL;
	secondaryBuffer[MAX_LINE_LENGTH - 1] = NULL;

	Read();
	Solve1();

	// for the second part I saw the repetitive pattern of the example and manually calculated the number.

	fin.close();
	fout.close();

	return 0;
}