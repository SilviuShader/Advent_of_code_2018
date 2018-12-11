#include <iostream>
#include <fstream>

#define MATRIX_SIZE 300

using namespace std;

ifstream fin;
ofstream fout;

int serialNumber;

int cellMatrix[MATRIX_SIZE + 2][MATRIX_SIZE + 2] = { 0 };
int sumMatrix[MATRIX_SIZE + 2][MATRIX_SIZE + 2] = { 0 };

void GenerateMatrix()
{
	for (int i = 1; i <= MATRIX_SIZE; i++)
	{
		for (int j = 1; j <= MATRIX_SIZE; j++)
		{
			int rackId = j + 10;
			int power = rackId * i;
			power += serialNumber;
			power *= rackId;
			int digit = (power / 100) % 10;
			digit -= 5;

			cellMatrix[i][j] = digit;
		}
	}
}

void GenerateSumMatrix()
{
	for (int i = 1; i <= MATRIX_SIZE; i++)
	{
		for (int j = 1; j <= MATRIX_SIZE; j++)
		{
			sumMatrix[i][j] = cellMatrix[i][j] + sumMatrix[i - 1][j] + sumMatrix[i][j - 1] - sumMatrix[i - 1][j - 1];
		}
	}
}

void Solve1()
{
	int maxValue = -(300 * 300);
	int solY = -1;
	int solX = -1;
	int solSize = -1;

	for (int k = 1; k <= MATRIX_SIZE; k++)
	{
		for (int i = 1; i <= MATRIX_SIZE - (k - 1); i++)
		{
			for (int j = 1; j <= MATRIX_SIZE - (k - 1); j++)
			{
				int yBottom = i + (k - 1);
				int xRight = j + (k - 1);

				int value = sumMatrix[yBottom][xRight] - sumMatrix[yBottom][j - 1] - sumMatrix[i - 1][xRight] + sumMatrix[i - 1][j - 1];

				if (value > maxValue)
				{
					maxValue = value;
					solY = i;
					solX = j;

					solSize = k;
				}
			}
		}
	}

	fout << solX << "," << solY << "," << solSize << endl;
}

int main(void)
{
	fin.open("serial.in");
	fout.open("serial.out");

	fin >> serialNumber;

	GenerateMatrix();
	GenerateSumMatrix();
	Solve1();

	fin.close();
	fout.close();

	return 0;
}