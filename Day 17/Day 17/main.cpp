#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include <conio.h>

#define MAX_MATRIX_SIZE 5000

using namespace std;

enum MatrixCellType
{
	Sand = 0,
	Clay = 1,
	DownWater = 2,
	SteadyWater = 3
};

struct WaterBlock
{
	WaterBlock(int i, int j) : y(i), x(j) { }
	int y, x;
};

ifstream fin("puzzle.in");
ofstream fout("puzzle.out");

MatrixCellType scannedMatrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = { MatrixCellType::Sand };
bool reachedEnd[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = { false };

bool calledFunc[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = { false };
bool resultsPerCall[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = { false };

int matrixMaxY = 0;
int matrixMaxX = 0;

int matrixMinX = MAX_MATRIX_SIZE;
int matrixMinY = MAX_MATRIX_SIZE;

vector<WaterBlock> waterBlocks;

void Read()
{
	char dumb;
	char character;
	int x;
	int startY;
	int endY;

	memset(scannedMatrix, MatrixCellType::Sand, sizeof(MatrixCellType) * MAX_MATRIX_SIZE * MAX_MATRIX_SIZE);
	memset(reachedEnd, false, sizeof(bool) * MAX_MATRIX_SIZE * MAX_MATRIX_SIZE);

	while (fin >> character >> dumb >> x >> dumb >> dumb >> dumb >> startY >> dumb >> dumb >> endY)
	{
		bool isX = character == 'x';
		
		for (int y = startY; y <= endY; y++)
		{
			matrixMaxX = max(matrixMaxX, (isX ? x : y));
			matrixMinX = min(matrixMinX, (isX ? x : y));

			matrixMaxY = max(matrixMaxY, (isX ? y : x));
			matrixMinY = min(matrixMinY, (isX ? y : x));

			if (y < MAX_MATRIX_SIZE && x < MAX_MATRIX_SIZE)
			{
				if (isX)
				{
					scannedMatrix[y][x] = MatrixCellType::Clay;
				}
				else
				{
					scannedMatrix[x][y] = MatrixCellType::Clay;
				}
			}
			else
			{
				cout << "Input data too big, this program will fail!!!" << endl;
			}
		}
	}

	scannedMatrix[0][500] = MatrixCellType::DownWater;
}

void DebugMatrix()
{
	for (int y = 0; y <= matrixMaxY; y++)
	{
		for (int x = matrixMinX; x <= matrixMaxX; x++)
		{
			fout << scannedMatrix[y][x];
		}
		fout << endl;
	}
}

int CountResult()
{
	int result = 0;

	for (int i = matrixMinY; i <= matrixMaxY; i++)
	{
		for (int j = 0; j < MAX_MATRIX_SIZE; j++)
		{
			if (scannedMatrix[i][j] == MatrixCellType::DownWater || scannedMatrix[i][j] == MatrixCellType::SteadyWater)
				result++;
		}
	}

	return result;
}

int CountResult2()
{
	int result = 0;

	for (int i = matrixMinY; i <= matrixMaxY; i++)
	{
		for (int j = 0; j < MAX_MATRIX_SIZE; j++)
		{
			if (scannedMatrix[i][j] == MatrixCellType::SteadyWater)
				result++;
		}
	}

	return result;
}

bool UpdateStep()
{
	bool result = false;

	int firstSize = waterBlocks.size();

	for(int l = 0; l < firstSize; l++)
	{
		int i = waterBlocks[l].y;
		int j = waterBlocks[l].x;

		if (i >= 0 && j >= 1 && i < MAX_MATRIX_SIZE - 1 && j < MAX_MATRIX_SIZE - 1)
		{
			if (scannedMatrix[i][j] == MatrixCellType::DownWater)
			{
				if (scannedMatrix[i + 1][j] == MatrixCellType::Sand)
				{
					scannedMatrix[i + 1][j] = MatrixCellType::DownWater;
					waterBlocks.push_back(WaterBlock(i + 1, j));
					result = true;
				}
				else
				{
					if (scannedMatrix[i + 1][j] == MatrixCellType::Clay || scannedMatrix[i + 1][j] == MatrixCellType::SteadyWater)
					{
						if (scannedMatrix[i][j + 1] == MatrixCellType::Sand)
						{
							scannedMatrix[i][j + 1] = MatrixCellType::DownWater;
							waterBlocks.push_back(WaterBlock(i, j + 1));
							result = true;
						}
						if (scannedMatrix[i][j - 1] == MatrixCellType::Sand)
						{
							scannedMatrix[i][j - 1] = MatrixCellType::DownWater;
							waterBlocks.push_back(WaterBlock(i, j - 1));
							result = true;
						}

						bool shouldBeSteady = true;

						for (int k = j - 1; scannedMatrix[i][k] != MatrixCellType::Clay && scannedMatrix[i][k] != MatrixCellType::SteadyWater && k >= 0; k--)
						{
							if (scannedMatrix[i][k] == MatrixCellType::Sand)
								shouldBeSteady = false;
						}

						for (int k = j + 1; k < MAX_MATRIX_SIZE && scannedMatrix[i][k] != MatrixCellType::Clay && scannedMatrix[i][k] != MatrixCellType::SteadyWater; k++)
						{
							if (scannedMatrix[i][k] == MatrixCellType::Sand)
								shouldBeSteady = false;
						}

						if (shouldBeSteady)
						{
							scannedMatrix[i][j] = MatrixCellType::SteadyWater;
							result = true;
						}
					}
				}
			}
		}
	}

	return result;
}

void Solve()
{
	WaterBlock waterBlock(0, 500);

	waterBlocks.push_back(waterBlock);

	while (UpdateStep())
	{
	}

	fout << CountResult() << endl << CountResult2() << endl;
}

int main(void)
{
	Read();

	Solve();
	
	fin.close();
	fout.close();
	
	return 0;
}