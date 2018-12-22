#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>

#define MAX_MATRIX_SIZE 2000

using namespace std;

enum Equipment
{
	Torch = 0,
	ClimbingGear = 1,
	Neither = 2
};

struct MatrixCell
{
	int geologicIndex;
	int toolValues[3] = { -1, -1, -1 };
};

struct PlayerPosition
{
	int y;
	int x;
	Equipment equipment;
	int accumulatedTime;
};

class CompareQueue
{
public:

	bool operator()(PlayerPosition player1, PlayerPosition player2)
	{
		return player1.accumulatedTime > player2.accumulatedTime;
	}
};

ifstream fin("puzzle.in");
ofstream fout("puzzle.out");

int depth;

int targetX;
int targetY;

int erosionMatrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE] = { 0 };
MatrixCell leeMatrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];

priority_queue<PlayerPosition, vector<PlayerPosition>, CompareQueue> leeQueue;

void Read()
{
	string dumb;
	char chr;
	fin >> dumb >> depth;
	fin >> dumb >> targetX >> chr >> targetY;
}

bool ValidPosition(PlayerPosition& playerPos)
{
	if (playerPos.x >= 0 && playerPos.x < MAX_MATRIX_SIZE &&
		playerPos.y >= 0 && playerPos.y < MAX_MATRIX_SIZE)
	{
		return true;
	}

	return false;
}

void Solve2()
{
	int dirY[] = { 0, -1, 0, 1 };
	int dirX[] = { -1, 0, 1, 0 };

	int result2 = 0;

	PlayerPosition playerPos;
	playerPos.x = 0;
	playerPos.y = 0;
	playerPos.equipment = Equipment::Torch;
	playerPos.accumulatedTime = 0;

	leeMatrix[0][0].toolValues[(int)Equipment::Torch] = 0;

	leeQueue.emplace(playerPos);

	while (!leeQueue.empty())
	{
		playerPos = leeQueue.top();
		leeQueue.pop();

		int timeUntil = leeMatrix[playerPos.y][playerPos.x].toolValues[(int)playerPos.equipment];
		int geologicIndex = leeMatrix[playerPos.y][playerPos.x].geologicIndex;

		vector<PlayerPosition> positionsThisPlace;

		PlayerPosition newPosThisPlace;
		newPosThisPlace.y = playerPos.y;
		newPosThisPlace.x = playerPos.x;
		newPosThisPlace.accumulatedTime = playerPos.accumulatedTime + 7;

		switch (geologicIndex)
		{
		case 0:
			if (playerPos.equipment == Equipment::ClimbingGear)
				newPosThisPlace.equipment = Equipment::Torch;
			else
				newPosThisPlace.equipment = Equipment::ClimbingGear;
			break;

		case 1:
			if (playerPos.equipment == Equipment::ClimbingGear)
				newPosThisPlace.equipment = Equipment::Neither;
			else
				newPosThisPlace.equipment = Equipment::ClimbingGear;
			break;

		case 2:
			if (playerPos.equipment == Equipment::Torch)
				newPosThisPlace.equipment = Equipment::Neither;
			else
				newPosThisPlace.equipment = Equipment::Torch;

			break;
		}

		positionsThisPlace.push_back(newPosThisPlace);
		positionsThisPlace.push_back(playerPos);

		for (int i = 0; i < 2; i++)
		{
			PlayerPosition thisPlace = positionsThisPlace[i];

			for (int k = 0; k < 4; k++)
			{
				PlayerPosition newPosition = thisPlace;

				newPosition.y += dirY[k];
				newPosition.x += dirX[k];

				newPosition.accumulatedTime++;

				if (ValidPosition(newPosition))
				{
					if (leeMatrix[newPosition.y][newPosition.x].toolValues[(int)newPosition.equipment] == -1 || leeMatrix[newPosition.y][newPosition.x].toolValues[(int)newPosition.equipment] > newPosition.accumulatedTime)
					{
						bool compatible = false;
						switch (leeMatrix[newPosition.y][newPosition.x].geologicIndex)
						{
						case 0:
							if (newPosition.equipment == Equipment::ClimbingGear || newPosition.equipment == Equipment::Torch)
								compatible = true;
							break;

						case 1:
							if (newPosition.equipment == Equipment::ClimbingGear || newPosition.equipment == Equipment::Neither)
								compatible = true;
							break;

						case 2:
							if (newPosition.equipment == Equipment::Torch || newPosition.equipment == Equipment::Neither)
								compatible = true;
							break;

						}

						if (compatible)
						{
							leeMatrix[newPosition.y][newPosition.x].toolValues[(int)newPosition.equipment] = newPosition.accumulatedTime;
							
							leeQueue.push(newPosition);
						}
					}
				}
			}
		}
	}

	fout << leeMatrix[targetY][targetX].toolValues[(int)Equipment::Torch] << endl;
}

void Solve()
{
	int result1 = 0;

	for (int y = 0; y < MAX_MATRIX_SIZE; y++)
	{
		for (int x = 0; x < MAX_MATRIX_SIZE; x++)
		{
			if (x == 0 && y == 0)
			{
				erosionMatrix[y][x] = 0;
			}
			else if (y == 0)
			{
				erosionMatrix[y][x] = x * 16807;
			} 
			else if (x == 0)
			{
				erosionMatrix[y][x] = y * 48271;
			}
			else if (x == targetX && y == targetY)
			{
				erosionMatrix[y][x] = 0;
			}
			else
			{
				erosionMatrix[y][x] = erosionMatrix[y - 1][x] * erosionMatrix[y][x - 1];
			}

			erosionMatrix[y][x] += depth;
			erosionMatrix[y][x] %= 20183;

			if (y <= targetY && x <= targetX)
			{
				result1 += erosionMatrix[y][x] % 3;
			}

			leeMatrix[y][x].geologicIndex = erosionMatrix[y][x] % 3;

			leeMatrix[y][x].toolValues[0] = -1;
			leeMatrix[y][x].toolValues[1] = -1;
			leeMatrix[y][x].toolValues[2] = -1;
		}
	}

	fout << result1 << endl;

	Solve2();
}

int main(void)
{
	Read();
	Solve();

	fin.close();
	fout.close();

	return 0;
}