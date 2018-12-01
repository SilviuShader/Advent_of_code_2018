#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 300
#define MAX_NUMBERS_COUNT 2000
#define MAX_RECCURENT_VALUE 800000

typedef long long LL;

FILE* fin;
FILE* fout;

char line[MAX_LINE_LENGTH] = { NULL };
LL numbers[MAX_NUMBERS_COUNT] = { 0 };

char reccurentNumbers[MAX_RECCURENT_VALUE] = { 0 };

int numNumbers = 0;

LL sum = 0;

int main(void)
{
	fin = fopen("day1.in", "r");
	fout = fopen("day1.out", "w");

	while (fscanf(fin, "%s", line) != EOF)
	{
		int len = strlen(line);

		if (len != 0)
		{
			LL sign = line[0] == '+' ? 1 : -1;
			LL number = 0;

			for (int i = 1; i < len; i++)
			{
				number = (number * (LL)10) + (LL)(line[i] - '0');
			}

			number *= sign;

			numbers[numNumbers++] = number;

			sum += number;
		}
	}

	fprintf(fout, "%lld\n", sum);

	if (numNumbers > 0)
	{
		LL sum2 = 0;
		int cond = 1;

		while (cond)
		{
			for (int i = 0; i < numNumbers; i++)
			{
				sum2 += numbers[i];

				LL index = sum2;
				int cond2 = (index < (MAX_RECCURENT_VALUE / 2));

				if (index < 0)
				{
					index = MAX_RECCURENT_VALUE + index;

					cond2 = (index >= (MAX_RECCURENT_VALUE / 2));
				}

				if (cond2)
				{
					if (reccurentNumbers[index] == 1)
					{
						fprintf(fout, "%lld\n", index);
						cond = 0;

						break;
					}

					reccurentNumbers[index] = 1;
				}
				else
				{
					printf("Houston, we have a problem");
				}
			}
		}
	}

	fclose(fin);
	fclose(fout);

	return 0;
}