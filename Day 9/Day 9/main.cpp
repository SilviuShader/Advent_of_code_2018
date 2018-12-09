#include <fstream>

using namespace std;

struct MarbleMember
{
	unsigned long long value = 0;

	MarbleMember* next = NULL;
	MarbleMember* prev = NULL;
};

ifstream fin;
ofstream fout;

int numPlayers;
unsigned long long lastMarble;

MarbleMember* circle = NULL;
unsigned long long* players;

void ReadData()
{
	char dummy[512] = { 0 };
	fin >> numPlayers >> dummy >> dummy >> dummy >> dummy >> dummy >> lastMarble;
}

unsigned long long Solve1()
{
	unsigned long long result1 = 0;
	circle = new MarbleMember();

	players = new unsigned long long[numPlayers];

	memset(players, 0, sizeof(unsigned long long) * numPlayers);

	MarbleMember* currentMember = circle;
	int crtPlayer = 0;

	for (int i = 1; i <= lastMarble; i++)
	{
		if (i % 23 != 0)
		{
			MarbleMember* nextMember = new MarbleMember();
			nextMember->value = i;
			
			if (i == 1)
			{
				nextMember->prev = currentMember;
				nextMember->next = currentMember;
				currentMember->next = nextMember;
				currentMember->prev = nextMember;
			}
			else
			{
				MarbleMember* nextPrev = currentMember->next;

				nextMember->prev = nextPrev;
				nextMember->next = nextPrev->next;

				nextPrev->next = nextMember;
				nextMember->next->prev = nextMember;
			}

			currentMember = nextMember;
		}
		else
		{
			players[crtPlayer] += i;
			MarbleMember* toRemove = currentMember->prev->prev->prev->prev->prev->prev->prev;
			players[crtPlayer] += toRemove->value;

			MarbleMember* next = toRemove->next;
			MarbleMember* prev = toRemove->prev;

			next->prev = prev;
			prev->next = next;

			delete toRemove;

			currentMember = next;
		}

		crtPlayer++;
		crtPlayer %= numPlayers;
	}

	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i] > result1)
			result1 = players[i];
	}

	return result1;
}

void DeleteList()
{
	while (circle)
	{
		if (circle->prev)
			circle->prev->next = NULL;
		if (circle->next)
			circle->next->prev = NULL;

		MarbleMember* next = circle->next;

		delete circle;

		circle = next;
	}
}

int main(void)
{
	fin.open("input.in");
	fout.open("output.out");

	ReadData();

	fout << Solve1();

	delete[] players;
	DeleteList();

	fin.close();
	fout.close();

	return 0;
}