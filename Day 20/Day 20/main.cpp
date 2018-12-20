// Maybe there are many things wrong with my code.. but it works on the samples and the puzzle input I recieved
// This puzzle was a f*cking TRACTOR.

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>

#define MAX_LINE_LENGHT 100000
#define MAX_BRACKETS    100000

using namespace std;

struct Brackets
{
	int startPos;
	int endPos;
};

struct Position
{
	int y, x;
	bool door = false;

	friend bool operator < (const Position& l, const Position& r) 
	{
		return (l.x < r.x || (l.x == r.x && l.y < r.y));
	}

	friend bool operator == (const Position& l, const Position& r)
	{
		return (l.x == r.x) && (l.y == r.y);
	}
};

struct DFSArguments
{
	int startBrackets; 
	int endBrackets; 
	int startDirections; 
	int depth;
	Position currentPosition;

	DFSArguments(int a, int b, int c, int d, Position e) :
		startBrackets(a),
		endBrackets(b),
		startDirections(c),
		depth(d),
		currentPosition(e)
	{

	}

	friend bool operator == (const DFSArguments& l, const DFSArguments& r)
	{
		return (l.startBrackets == r.startBrackets) && (l.endBrackets == r.endBrackets) && (l.startDirections == r.startDirections) && (l.depth == r.depth) && (l.currentPosition == r.currentPosition);
	}

	friend bool operator < (const DFSArguments& l, const DFSArguments& r)
	{
		if (l.startBrackets < r.startBrackets)
			return true;
		else
		{
			if (l.startBrackets == r.startBrackets)
			{
				if (l.endBrackets < r.endBrackets)
					return true;
				else
				{
					if (l.endBrackets == r.endBrackets)
					{
						if (l.startDirections < r.startDirections)
							return true;
						else
						{
							if (l.startDirections == r.startDirections)
							{
								if (l.depth < r.depth)
									return true;
								else
								{
									if (l.depth == r.depth)
									{
										if (l.currentPosition < r.currentPosition)
											return true;
									}
								}
							}
						}
					}
				}
			}
		}

		return false;
	}
};

struct MatrixCell
{
	bool canVisit = false;
	int distance = -1;
	bool door = false;
};

int dirY[] = { 0, -1, 0, 1 };
int dirX[] = { -1, 0, 1, 0 };

ifstream fin("puzzle.in");
ofstream fout("puzzle.out");

char inputData[MAX_LINE_LENGHT] = { NULL };
int dataLength;

Brackets brackets[MAX_BRACKETS];
vector<Position> roomPositions;

map<Position, bool> positionsMap;

map<int, int> bracketStartMap;
map<int, vector<int> > bracketMapPositions;
map<DFSArguments, bool> callMap;

MatrixCell** roomsMatrix = NULL;
int roomMatrixHeight = 0;
int roomMatrixWidth = 0;

queue<Position> leeQueue;

void Read()
{
	fin.getline(inputData, MAX_LINE_LENGHT);
	dataLength = strlen(inputData);

	inputData[0] = '(';
	inputData[dataLength - 1] = ')';
}

void DFS(int startBrackets, int endBrackets, int startDirections, int depth, Position currentPosition)
{
	int indexPos = 0;

	if (callMap.find(DFSArguments(startBrackets, endBrackets, startDirections, depth, currentPosition)) == callMap.end())
	{
		for (indexPos = startDirections; inputData[indexPos] >= 'A' && inputData[indexPos] <= 'Z'; indexPos++)
		{
			for (int i = 0; i < 2; i++)
			{
				switch (inputData[indexPos])
				{
				case 'N':
					currentPosition.y--;
					break;
				case 'S':
					currentPosition.y++;
					break;
				case 'E':
					currentPosition.x++;
					break;
				case 'W':
					currentPosition.x--;
					break;
				default:
					cout << "WAT?\n";
					break;
				}

				if (positionsMap.find(currentPosition) == positionsMap.end())
				{
					roomPositions.push_back(currentPosition);
					roomPositions[roomPositions.size() - 1].door = (bool)(1 - i);
					positionsMap[currentPosition] = true;
				}
				else
				{
				}
			}
		}

		if (indexPos == endBrackets || inputData[indexPos] == '|')
		{
			if (depth != 0)
			{
				Brackets prevBrackets = brackets[depth - 1];
				DFS(prevBrackets.startPos, prevBrackets.endPos, endBrackets + 1, depth - 1, currentPosition);
			}
		}

		if (inputData[indexPos] == '(')
		{
			int grade = 1;
			int newEndPos = indexPos;

			if (bracketStartMap.find(indexPos) == bracketStartMap.end())
			{
				for (int i = indexPos + 1; grade > 0; i++)
				{
					if (inputData[i] == '(')
						grade++;
					if (inputData[i] == ')')
						grade--;

					if (grade == 0)
					{
						newEndPos = i;
					}
				}

				bracketStartMap[indexPos] = newEndPos;
			}
			else
			{
				newEndPos = bracketStartMap[indexPos];
			}

			grade = 1;
			bool prevOr = true;

			vector<int> positionsToCall;

			if (bracketMapPositions.find(indexPos) == bracketMapPositions.end())
			{
				for (int i = indexPos + 1; grade > 0; i++)
				{
					if (prevOr && grade == 1)
					{
						brackets[depth].startPos = startBrackets;
						brackets[depth].endPos = endBrackets;

						positionsToCall.push_back(i);
					}

					prevOr = false;

					if (inputData[i] == '(')
						grade++;
					if (inputData[i] == ')')
						grade--;

					if (inputData[i] == '|')
						prevOr = true;
				}

				bracketMapPositions[indexPos] = positionsToCall;
			}
			else
			{
				positionsToCall = bracketMapPositions[indexPos];
			}

			for (int i = 0; i < positionsToCall.size(); i++)
			{
				brackets[depth].startPos = startBrackets;
				brackets[depth].endPos = endBrackets;

				DFS(indexPos, newEndPos, positionsToCall[i], depth + 1, currentPosition);
			}
		}

		callMap[DFSArguments(startBrackets, endBrackets, startDirections, depth, currentPosition)] = true;
	}
}

void DebugPositions()
{
	for (int i = 0; i < roomPositions.size(); i++)
	{
		cout << roomPositions[i].y << " " << roomPositions[i].x << endl;
	}
}

Position CreateMatrix()
{
	int minY = 0;
	int minX = 0;

	Position pivot;

	for (int i = 0; i < roomPositions.size(); i++)
	{
		if (roomPositions[i].y < minY)
			minY = roomPositions[i].y;

		if (roomPositions[i].x < minX)
			minX = roomPositions[i].x;
	}

	for (int i = 0; i < roomPositions.size(); i++)
	{
		roomPositions[i].y -= minY;
		roomPositions[i].x -= minX;
	}

	pivot.y = -minY;
	pivot.x = -minX;

	int maxY = 0;
	int maxX = 0;

	for (int i = 0; i < roomPositions.size(); i++)
	{
		if (roomPositions[i].y > maxY)
			maxY = roomPositions[i].y;

		if (roomPositions[i].x > maxX)
			maxX = roomPositions[i].x;
	}

	roomMatrixHeight = maxY + 1;
	roomMatrixWidth = maxX + 1;

	roomsMatrix = new MatrixCell*[roomMatrixHeight];

	for (int i = 0; i < roomMatrixHeight; i++)
	{
		roomsMatrix[i] = new MatrixCell[roomMatrixWidth];

		for (int j = 0; j < roomMatrixWidth; j++)
		{
			roomsMatrix[i][j].canVisit = false;
			roomsMatrix[i][j].distance = -1;
			roomsMatrix[i][j].door = false;
		}
	}

	for (int i = 0; i < roomPositions.size(); i++)
	{
		roomsMatrix[roomPositions[i].y][roomPositions[i].x].canVisit = true;
		roomsMatrix[roomPositions[i].y][roomPositions[i].x].door = roomPositions[i].door;
	}

	return pivot;
}

void DebugVisitMatrix()
{
	for (int i = 0; i < roomMatrixHeight; i++)
	{
		for (int j = 0; j < roomMatrixWidth; j++)
		{
			cout << roomsMatrix[i][j].canVisit;
		}
		cout << endl;
	}
}

bool ValidCoord(Position coord)
{
	if (coord.y >= 0 && coord.y < roomMatrixHeight &&
		coord.x >= 0 && coord.x < roomMatrixWidth)
	{
		return true;
	}

	return false;
}

void Lee(Position coord)
{
	roomsMatrix[coord.y][coord.x].distance = 0;
	leeQueue.push(coord);

	while (!leeQueue.empty())
	{
		Position actualCoord = leeQueue.front();
		leeQueue.pop();
		int actualDistance = roomsMatrix[actualCoord.y][actualCoord.x].distance;

		for (int i = 0; i < 4; i++)
		{
			Position newCoord = actualCoord;
			
			newCoord.y += dirY[i];
			newCoord.x += dirX[i];

			if (ValidCoord(newCoord))
			{
				if (roomsMatrix[newCoord.y][newCoord.x].canVisit)
				{
					int newDistance = actualDistance + 1;

					if (roomsMatrix[newCoord.y][newCoord.x].distance == -1 || roomsMatrix[newCoord.y][newCoord.x].distance > newDistance)
					{
						roomsMatrix[newCoord.y][newCoord.x].distance = newDistance;

						leeQueue.push(newCoord);
					}
				}
			}
		}
	}
}

void DebugDistances()
{
	for (int i = 0; i < roomMatrixHeight; i++)
	{
		for (int j = 0; j < roomMatrixWidth; j++)
		{
			cout << roomsMatrix[i][j].distance << " ";
		}
		cout << endl;
	}
}

void ShowResult1()
{
	int maxDist = -1;
	for (int i = 0; i < roomMatrixHeight; i++)
	{
		for (int j = 0; j < roomMatrixWidth; j++)
		{
			if (roomsMatrix[i][j].distance > maxDist)
				maxDist = roomsMatrix[i][j].distance;
		}
	}

	fout << maxDist / 2 << endl;
}

void ShowResult2()
{
	int num = 0;

	for (int i = 0; i < roomMatrixHeight; i++)
	{
		for (int j = 0; j < roomMatrixWidth; j++)
		{
			if (roomsMatrix[i][j].canVisit)
			{
				if (!roomsMatrix[i][j].door)
				{
					if ((roomsMatrix[i][j].distance / 2) >= 1000)
					{
						num++;
					}
				}
			}
		}
	}

	fout << num << endl;
}

void Solve()
{
	Position initialPosition;

	initialPosition.y = 0;
	initialPosition.x = 0;

	roomPositions.push_back(initialPosition);

	DFS(0, dataLength - 1, 1, 0, initialPosition);
	Position pivot = CreateMatrix();

	Lee(pivot);

	ShowResult1();
	ShowResult2();
}

int main(void)
{
	Read();
	Solve();

	fin.close();
	fout.close();

	for (int i = 0; i < roomMatrixHeight; i++)
	{
		delete[] roomsMatrix[i];
		roomsMatrix[i] = NULL;
	}

	delete[] roomsMatrix;
	roomsMatrix = NULL;

	return 0;
}