#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <conio.h>

#define MAX_MATRIX_WIDTH 1000

using namespace std;

enum CartOrientation
{
	Up = 0,
	Right = 1,
	Down = 2,
	Left = 3
};

struct CartDetails
{
	int posX;
	int posY;

	int dirX;
	int dirY;

	int intersectionIndex = 0;

	CartOrientation orientation;

	bool active = true;

	void TurnLeft()
	{
		int dir = (int)orientation;
		dir--;
		
		if (dir <= -1)
			dir += 4;

		orientation = (CartOrientation)dir;

		UpdateDirVector();
	}

	void TurnRight()
	{
		int dir = (int)orientation;
		dir++;
		dir = dir % 4;
		orientation = (CartOrientation)dir;

		UpdateDirVector();
	}

	void UpdateDirVector()
	{
		switch (orientation)
		{
		case Up:
			dirX = 0;
			dirY = -1;
			break;
		case Right:
			dirX = 1;
			dirY = 0;
			break;
		case Down:
			dirX = 0;
			dirY = 1;
			break;
		case Left:
			dirX = -1;
			dirY = 0;
			break;
		}
	}

	void DoIntersection()
	{
		switch (intersectionIndex)
		{
		case 0:
			TurnLeft();
			break;
		case 1:
			break;
		case 2:
			TurnRight();
			break;
		}
		intersectionIndex++;
		intersectionIndex %= 3;
	}

	void DoCorner(char corner)
	{
		switch (orientation)
		{
		case CartOrientation::Up:
			if (corner == '\\')
				TurnLeft();
			else
				TurnRight();
			break;
		case CartOrientation::Right:
			if (corner == '\\')
				TurnRight();
			else
				TurnLeft();
			break;
		case CartOrientation::Down:
			if (corner == '\\')
				TurnLeft();
			else
				TurnRight();
			break;
		case CartOrientation::Left:
			if (corner == '\\')
				TurnRight();
			else
				TurnLeft();
			break;
		}
	}

	void Step()
	{
		posX += dirX;
		posY += dirY;
	}
};

char trackMatrix[MAX_MATRIX_WIDTH][MAX_MATRIX_WIDTH] = { 0 };
int matrixLinesCount = 0;
int lineLength = 0;
int activeCarts = 0;

ifstream fin("tracks.in");
ofstream fout("tracks.out");

vector<CartDetails> carts;

void Read()
{
	while (fin.getline(&trackMatrix[matrixLinesCount][0], MAX_MATRIX_WIDTH))
	{
		int length = strlen(&trackMatrix[matrixLinesCount][0]);

		for (int i = 0; i < length; i++)
		{
			char currentChar = trackMatrix[matrixLinesCount][i];
			if (currentChar == '^' || currentChar == 'v' || currentChar == '<' || currentChar == '>')
			{
				CartDetails cart;
				cart.posX = i;
				cart.posY = matrixLinesCount;

				switch (currentChar)
				{
				case '^':
					cart.orientation = CartOrientation::Up;
					break;
				case 'v':
					cart.orientation = CartOrientation::Down;
					break;
				case '<':
					cart.orientation = CartOrientation::Left;
					break;
				case '>':
					cart.orientation = CartOrientation::Right;
					break;
				}

				cart.UpdateDirVector();

				carts.push_back(cart);
			}
		}

		matrixLinesCount++;
	}

	lineLength = strlen(&trackMatrix[0][0]);
}

void DebugMatrix()
{
	for (int i = 0; i < matrixLinesCount; i++)
	{
		for (int j = 0; j < lineLength; j++)
		{
			cout << trackMatrix[i][j];
		}
		cout << endl;
	}
}

bool CheckAccident(int cartIndex, int part)
{
	for (int i = 0; i < carts.size(); i++)
	{
		if (carts[i].active)
		{
			if (cartIndex != i)
			{
				if (carts[cartIndex].posX == carts[i].posX && carts[cartIndex].posY == carts[i].posY)
				{
					if (part == 2)
					{
						carts[i].active = false;
						activeCarts--;
					}

					return true;
				}
			}
		}
	}

	return false;
}

bool Step(int cartIndex, int part)
{
	CartDetails& cart = carts[cartIndex];

	switch (trackMatrix[cart.posY][cart.posX])
	{
	case '+':
		cart.DoIntersection();
		break;
	case '\\':
		cart.DoCorner(trackMatrix[cart.posY][cart.posX]);
		break;
	case '/':
		cart.DoCorner(trackMatrix[cart.posY][cart.posX]);
		break;
	}

	cart.Step();

	if (part == 1)
	{
		if (CheckAccident(cartIndex, part))
			return false;
	}

	if (part == 2)
	{
		if (CheckAccident(cartIndex, part))
		{
			cart.active = false;
			activeCarts--;
		}
	}

	return true;
}

void DoTrick()
{
	sort(carts.begin(), carts.end(), [](CartDetails cart1, CartDetails cart2)
	{
		if (cart1.posY == cart2.posY)
			return cart1.posX < cart2.posX;

		return cart1.posY < cart2.posY;
	});
}

void Solve()
{
	bool canContinue = true;
	int cartIndex = 0;
	int accidentCartIndex = 0;

	do
	{
		canContinue = Step(cartIndex, 1);

		if (!canContinue)
		{
			accidentCartIndex = cartIndex;
		}
		
		cartIndex++;
		cartIndex %= carts.size();

		if (cartIndex == 0)
		{
			DoTrick();
		}

	} while (canContinue);

	fout << carts[accidentCartIndex].posX << "," << carts[accidentCartIndex].posY << endl;
}

void Solve2()
{
	activeCarts = carts.size();

	int cartIndex = 0;
	bool cond = true;

	do
	{
		if (carts[cartIndex].active)
		{
			Step(cartIndex, 2);
		}

		cartIndex++;
		cartIndex %= carts.size();

		if (cartIndex == 0)
		{
			DoTrick();
			if (activeCarts == 1)
			{
				cond = false;
			}
		}

	} while (cond);

	for (int i = 0; i < carts.size(); i++)
	{
		if (carts[i].active)
		{
			fout << carts[i].posX << "," << carts[i].posY;
		}
	}
}

int main(void)
{
	Read();
	
	Solve();

	fin.close();
	carts.clear();
	matrixLinesCount = 0;
	fin.open("tracks.in");
	Read();

	Solve2();

	fin.close();
	fout.close();

	return 0;
}