#include <iostream>
#include <fstream>

#include <conio.h>

#include <vector>

#define NUMS_PLUS_COUNT 10

using namespace std;

ifstream fin("puzzle.in");
ofstream fout("puzzle.out");

int recipesCount = 0;

vector<int> recipesAdded;
int index1 = 0, index2 = 1;

int GotTheResult(int targetDigits)
{
	int number = 0;
	for (int i = recipesAdded.size() - targetDigits; i < recipesAdded.size(); i++)
	{
		number = (number * 10) + recipesAdded[i];
	}

	return number == recipesCount ? recipesAdded.size() - targetDigits : -1;
}

int AddStep(int part, int targetDigits)
{
	int result = recipesAdded[index1] + recipesAdded[index2];

	if (result >= 10)
	{
		recipesAdded.push_back(result / 10);

		if (part == 2)
		{
			if (GotTheResult(targetDigits) != -1)
			{
				return GotTheResult(targetDigits);
			}
		}
	}

	recipesAdded.push_back(result % 10);

	if (part == 2)
	{
		if (GotTheResult(targetDigits) != -1)
		{
			return GotTheResult(targetDigits);
		}
	}

	index1 += recipesAdded[index1] + 1;
	index1 %= recipesAdded.size();

	index2 += recipesAdded[index2] + 1;
	index2 %= recipesAdded.size();

	return -1;
}

void Solve1()
{
	recipesAdded.push_back(3);
	recipesAdded.push_back(7);

	while (recipesAdded.size() < recipesCount + NUMS_PLUS_COUNT)
	{
		AddStep(1, 0);
	}

	for (int i = recipesCount; i < recipesCount + NUMS_PLUS_COUNT; i++)
	{
		fout << recipesAdded[i];
	}

	fout << endl;
}

int NumDigits(int a)
{
	int dig = 0;
	while (a)
	{
		dig++;
		a /= 10;
	}

	return dig;
}

void Solve2(int tDig)
{
	recipesAdded.push_back(3);
	recipesAdded.push_back(7);

	bool cond = true;

	int value = -1;
	int targetDigits = tDig;

	while (cond)
	{
		value = AddStep(2, targetDigits);
		
		if (value != -1)
		{
			cond = false;
		}
	}
	
	fout << value << endl;
}

int main(void)
{
	char input[NUMS_PLUS_COUNT] = { 0 };
	fin >> input;

	fin.close();

	fin.open("puzzle.in");
	fin >> recipesCount;

	Solve1();

	recipesAdded.clear();
	index1 = 0;
	index2 = 1;

	Solve2(strlen(input));

	fin.close();
	fout.close();

	return 0;
}