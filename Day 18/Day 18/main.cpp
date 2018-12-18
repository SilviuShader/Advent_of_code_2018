#include <iostream>
#include <fstream>

#define MAX_MATRIX_SIZE 505

using namespace std;

struct NeighbourSection 
{
	int freeSpace = 0;
	int trees = 0;
	int lumberjack = 0;
};

ifstream fin("puzzle.in");
ofstream fout("puzzle.out");

char initialMatrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = { NULL };
char secondaryMatrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = { NULL };

int wasResult[50 * 50 * 50 * 50] = { NULL };

int lineCount = 0;
int lineWidth = 0;

void Read()
{
	while (fin.getline(&initialMatrix[lineCount][0], MAX_MATRIX_SIZE))
	{
		lineCount++;
	}

	lineWidth = strlen(&initialMatrix[0][0]);
}

bool ValidPos(int y, int x)
{
	if (y >= 0 && y < lineCount &&
		x >= 0 && x < lineWidth)
		return true;

	return false;
}

NeighbourSection GetNeighbours(char(*dataMatrix)[MAX_MATRIX_SIZE], int y, int x)
{
	NeighbourSection result;

	int dirY[] = { -1, 0, 1, 0, -1, 1, -1, 1 };
	int dirX[] = { 0, -1, 0, 1, -1, 1, 1, -1 };

	for (int i = 0; i < 8; i++)
	{
		int nY = dirY[i] + y;
		int nX = dirX[i] + x;

		if (ValidPos(nY, nX))
		{
			switch (dataMatrix[nY][nX])
			{
			case '.':
				result.freeSpace++;
				break;
			case '|':
				result.trees++;
				break;
			case '#':
				result.lumberjack++;
				break;
			}
		}
	}

	return result;
}

void Step(char(*dataMatrix)[MAX_MATRIX_SIZE], char(*workMatrix)[MAX_MATRIX_SIZE])
{
	for (int i = 0; i < lineCount; i++)
	{
		for (int j = 0; j < lineWidth; j++)
		{
			workMatrix[i][j] = dataMatrix[i][j];

			NeighbourSection neighbours = GetNeighbours(dataMatrix, i, j);

			if (dataMatrix[i][j] == '.')
			{
				if (neighbours.trees >= 3)
					workMatrix[i][j] = '|';
			}

			if (dataMatrix[i][j] == '|')
			{
				if (neighbours.lumberjack >= 3)
					workMatrix[i][j] = '#';
			}

			if (dataMatrix[i][j] == '#')
			{
				if (neighbours.trees >= 1 && neighbours.lumberjack >= 1)
					workMatrix[i][j] = '#';
				else
					workMatrix[i][j] = '.';
			}
		}
	}
}

void DebugMatrix(char(*dataMatrix)[MAX_MATRIX_SIZE])
{
	for (int i = 0; i < lineCount; i++)
	{
		for (int j = 0; j < lineWidth; j++)
		{
			cout << dataMatrix[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

int CountTrees(char(*dataMatrix)[MAX_MATRIX_SIZE])
{
	int numTrees = 0;
	int numLumberyard = 0;

	for (int i = 0; i < lineCount; i++)
	{
		for (int j = 0; j < lineWidth; j++)
		{
			if (dataMatrix[i][j] == '|')
			{
				numTrees++;
			}

			if (dataMatrix[i][j] == '#')
			{
				numLumberyard++;
			}
		}
	}

	return numTrees * numLumberyard;
}

void Solve()
{
	char (*dataMatrix)[MAX_MATRIX_SIZE] = initialMatrix;
	char (*workMatrix)[MAX_MATRIX_SIZE] = secondaryMatrix;

	for (int i = 0; i < 23000; i++)
	{
		Step(dataMatrix, workMatrix);

		char(*tmp)[MAX_MATRIX_SIZE] = dataMatrix;
		dataMatrix = workMatrix;
		workMatrix = tmp;

		int thisStepTrees = CountTrees(dataMatrix);

		if (i == 9)
		{
			fout << thisStepTrees << endl;
		}

		// done some math to find this number, it doesn't algorithmically recognize the pattern
		if (i + 1 == 22000)
		{
			fout << thisStepTrees << endl;
		}

		wasResult[thisStepTrees] = i;
	}
}

int main(void)
{
	Read();

	Solve();

	fin.close();
	fout.close();

	return 0;
}