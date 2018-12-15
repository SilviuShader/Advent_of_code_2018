#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

#include <conio.h>

#define MAX_LINES 64

using namespace std;

struct Coord
{
	int y, x;
};

struct Entity
{
	bool elf;
	Coord position;
	int health;

	int distances[MAX_LINES][MAX_LINES];
};

ifstream fin("puzzle.in");
ofstream fout("puzzle.out");

char combatMap[MAX_LINES][MAX_LINES] = { 0 };
int mapLines = 0;
int lineLength = 0;

int dirY[] = { -1, 0, 0, 1 };
int dirX[] = { 0, -1, 1, 0 };

vector<Entity> entitiesVector;

void Read()
{
	memset(combatMap, 0, sizeof(char) * MAX_LINES * MAX_LINES);
	while (fin.getline(&combatMap[mapLines][0], MAX_LINES))
	{
		int lgh = strlen(&combatMap[mapLines][0]);

		for (int i = 0; i < lgh; i++)
		{
			if (combatMap[mapLines][i] == 'E' || combatMap[mapLines][i] == 'G')
			{
				Entity newEntity;

				newEntity.elf = (combatMap[mapLines][i] == 'E');
				newEntity.health = 200;
				newEntity.position.y = mapLines;
				newEntity.position.x = i;
				memset(newEntity.distances, 0, sizeof(int) * MAX_LINES * MAX_LINES);

				entitiesVector.push_back(newEntity);
			}
		}

		mapLines++;
	}

	lineLength = strlen(&combatMap[0][0]);
}

void DebugMap()
{
	for (int i = 0; i < mapLines; i++)
	{
		for (int j = 0; j < lineLength; j++)
		{
			cout << combatMap[i][j];
		}
		cout << endl;
	}
}

void DebugEntities()
{
	for (int i = 0; i < entitiesVector.size(); i++)
	{
		cout << entitiesVector[i].position.y << " " << entitiesVector[i].position.x << " " << entitiesVector[i].health << " " << entitiesVector[i].elf << endl;
	}
}

bool ValidCoord(Coord coord)
{
	if (coord.y >= 0 && coord.y < mapLines &&
		coord.x >= 0 && coord.x < lineLength)
	{
		if (combatMap[coord.y][coord.x] != '#')
		{
			return true;
		}
	}

	return false;
}

void CalculateDistances(Entity& entity)
{
	queue<Coord> leeQueue;

	for (int i = 0; i < MAX_LINES; i++)
	{
		for (int j = 0; j < MAX_LINES; j++)
		{
			entity.distances[i][j] = -1;
		}
	}

	entity.distances[entity.position.y][entity.position.x] = 0;
	leeQueue.push(entity.position);

	while (!leeQueue.empty())
	{
		Coord crtCoord = leeQueue.front();
		leeQueue.pop();

		int crtValue = entity.distances[crtCoord.y][crtCoord.x];

		for (int i = 0; i < 4; i++)
		{
			Coord newCoord = crtCoord;
			newCoord.y += dirY[i];
			newCoord.x += dirX[i];

			int newValue = crtValue + 1;

			if (ValidCoord(newCoord))
			{
				if ((entity.distances[newCoord.y][newCoord.x] == -1) || entity.distances[newCoord.y][newCoord.x] > newValue)
				{
					entity.distances[newCoord.y][newCoord.x] = newValue;

					if (combatMap[newCoord.y][newCoord.x] == '.')
					{
						leeQueue.push(newCoord);
					}
				}
			}
		}
	}
}

void DebugEntityMatrix(Entity& entity)
{
	for (int i = 0; i < mapLines; i++)
	{
		for (int j = 0; j < lineLength; j++)
		{
			cout << entity.distances[i][j] << " ";
		}

		cout << endl;
	}
}

bool IsNeighbour(Coord origin, Coord neightbour)
{
	for (int i = 0; i < 4; i++)
	{
		Coord next = origin;
		next.y += dirY[i];
		next.x += dirX[i];

		if (next.y == neightbour.y && next.x == neightbour.x)
			return true;
	}

	return false;
}

void UpdateEntity(int elveDamage, int index)
{
	Entity& currentEntity = entitiesVector[index];

	if (currentEntity.health > 0)
	{
		CalculateDistances(currentEntity);

		int nextEntityIndex = -1;

		for (int i = 0; i < entitiesVector.size(); i++)
		{
			if (entitiesVector[i].elf != currentEntity.elf)
			{
				if (entitiesVector[i].health > 0)
				{
					if (currentEntity.distances[entitiesVector[i].position.y][entitiesVector[i].position.x] != -1)
					{
						if (nextEntityIndex == -1)
						{
							nextEntityIndex = i;
						}
						else
						{
							if (currentEntity.distances[entitiesVector[i].position.y][entitiesVector[i].position.x] <
								currentEntity.distances[entitiesVector[nextEntityIndex].position.y][entitiesVector[nextEntityIndex].position.x])
							{
								nextEntityIndex = i;
							}
						}
					}
				}
			}
		}

		if (nextEntityIndex != -1)
		{
			Entity& closestEntity = entitiesVector[nextEntityIndex];

			CalculateDistances(closestEntity);

			Coord nextCoord = currentEntity.position;

			for (int i = 0; i < 4; i++)
			{
				Coord newCoord = currentEntity.position;
				newCoord.y += dirY[i];
				newCoord.x += dirX[i];

				if (ValidCoord(newCoord))
				{
					if (closestEntity.distances[newCoord.y][newCoord.x] != -1)
					{
						if (closestEntity.distances[newCoord.y][newCoord.x] < closestEntity.distances[nextCoord.y][nextCoord.x])
						{
							if (combatMap[newCoord.y][newCoord.x] == '.')
							{
								nextCoord = newCoord;
							}
						}
					}
				}
			}

			if (combatMap[nextCoord.y][nextCoord.x] == '.')
			{
				combatMap[currentEntity.position.y][currentEntity.position.x] = '.';
				combatMap[nextCoord.y][nextCoord.x] = (currentEntity.elf ? 'E' : 'G');

				currentEntity.position = nextCoord;
			}
		}

		int minNeighbour = -1;

		for (int i = 0; i < entitiesVector.size(); i++)
		{
			if (entitiesVector[i].elf != currentEntity.elf)
			{
				if (entitiesVector[i].health > 0)
				{
					if (IsNeighbour(currentEntity.position, entitiesVector[i].position))
					{
						if (minNeighbour == -1)
						{
							minNeighbour = i;
						}
						else
						{
							if (entitiesVector[i].health < entitiesVector[minNeighbour].health)
							{
								minNeighbour = i;
							}
							else
							{
								if (entitiesVector[i].health == entitiesVector[minNeighbour].health)
								{
									if (entitiesVector[minNeighbour].position.y > entitiesVector[i].position.y)
									{
										minNeighbour = i;
									}
									else
									{
										if (entitiesVector[minNeighbour].position.y == entitiesVector[i].position.y)
										{
											if (entitiesVector[minNeighbour].position.x > entitiesVector[i].position.x)
											{
												minNeighbour = i;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		if (minNeighbour != -1)
		{
			if (currentEntity.elf)
			{
				entitiesVector[minNeighbour].health -= elveDamage;
			}
			else
			{
				entitiesVector[minNeighbour].health -= 3;
			}

			if (entitiesVector[minNeighbour].health <= 0)
			{
				combatMap[entitiesVector[minNeighbour].position.y][entitiesVector[minNeighbour].position.x] = '.';
			}
		}
	}
}

bool Solve1(int elveDamage, int part)
{
	bool cond = true;
	int entityIndex = 0;
	int iterations = 0;
	int healthSum = 0;

	while (cond)
	{
		UpdateEntity(elveDamage, entityIndex);

		entityIndex++;

		if (entityIndex >= entitiesVector.size())
		{
			entityIndex = 0;

			sort(entitiesVector.begin(), entitiesVector.end(), [&](Entity e1, Entity e2) 
			{
				if (e1.position.y == e2.position.y)
				{
					return e1.position.x < e2.position.x;
				}
				else
				{
					return e1.position.y < e2.position.y;
				}
			});

			//cout << endl;
			//DebugMap();

			iterations++;
		}

		cond = false;

		int prevValue = -1;

		for (int i = 0; i < entitiesVector.size(); i++)
		{
			if (prevValue == -1)
			{
				if (entitiesVector[i].health > 0)
					prevValue = (int)entitiesVector[i].elf;
			}
			else
			{
				if (entitiesVector[i].health > 0)
				{
					if ((int)entitiesVector[i].elf != prevValue)
						cond = true;

					prevValue = entitiesVector[i].elf;
				}
			}
		}

		if (part == 2)
		{
			for (int i = 0; i < entitiesVector.size(); i++)
			{
				if (entitiesVector[i].health < 0)
				{
					if (entitiesVector[i].elf)
					{
						return false;
					}
				}
			}
		}
	}

	for (int i = 0; i < entitiesVector.size(); i++)
	{
		if (entitiesVector[i].health > 0)
		{
			healthSum += entitiesVector[i].health;
		}
	}

	fout << iterations * healthSum << endl;

	return true;
}

void Solve2()
{
	bool cond = true;
	for (int i = 3; cond; i++)
	{
		fin.close();
		fin.open("puzzle.in");

		entitiesVector.clear();
		mapLines = 0;

		Read();

		cond = !Solve1(i, 2);
	}
}

int main(void)
{
	Read();
	
	Solve1(3, 1);

	Solve2();

	fin.close();
	fout.close();

	return 0;
}