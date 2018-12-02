#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define MAX_LETTER_COUNT 26

using namespace std;

ifstream fin;
ofstream fout;

vector<string> ids;

int letterAppearCount[MAX_LETTER_COUNT] = { 0 };

int Checksum()
{
	int numIds = ids.size();
	int numTwo = 0;
	int numThree = 0;

	bool addTwo = false;
	bool addThree = false;

	for (int i = 0; i < numIds; i++)
	{
		string& id = ids[i];
		int idLenght = id.size();

		memset(letterAppearCount, NULL, sizeof(int) * MAX_LETTER_COUNT);

		for (int j = 0; j < idLenght; j++)
		{
			int index = (int)(id.at(j) - 'a');
			letterAppearCount[index]++;
		}

		for (int j = 0; j < MAX_LETTER_COUNT; j++)
		{
			if (letterAppearCount[j] == 2)
			{
				addTwo = true;
			}

			if (letterAppearCount[j] == 3)
			{
				addThree = true;
			}
		}

		if (addTwo)
		{
			numTwo++;
		}

		if (addThree)
		{
			numThree++;
		}

		addTwo = false;
		addThree = false;
	}

	return numTwo * numThree;
}

string CommonLetters()
{
	string str = string();
	int numIds = ids.size();

	for (int i = 0; i < numIds; i++)
	{
		for (int j = i + 1; j < numIds; j++)
		{
			string& string1 = ids[i];
			string& string2 = ids[j];

			int lgh1 = string1.size();
			int lgh2 = string2.size();

			if (lgh1 == lgh2)
			{
				int numDiff = 0;

				for (int k = 0; k < lgh1; k++)
				{
					if (string1.at(k) != string2.at(k))
					{
						numDiff++;
					}
				}

				if (numDiff == 1)
				{
					for (int k = 0; k < lgh1; k++)
					{
						if (string1.at(k) == string2.at(k))
						{
							str += string1.at(k);
						}
					}
					return str;
				}
			}
		}
	}

	return str;
}

int main(void)
{
	string id;

	fin.open("ids.in");
	fout.open("ids.out");

	while (fin >> id)
	{
		ids.push_back(id);
	}

	fout << Checksum() << endl;
	fout << CommonLetters() << endl;

	fin.close();
	fout.close();

	return 0;
}