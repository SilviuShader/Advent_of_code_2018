#include <iostream>
#include <fstream>
#include <vector>

#include <conio.h>

#define MAX_LINE_LENGTH 500

using namespace std;

enum Operations
{
	Addr = 0,
	Addi = 1,
	Mulr = 2,
	Muli = 3,
	Banr = 4,
	Bani = 5,
	Borr = 6,
	Bori = 7,
	Setr = 8,
	Seti = 9,
	Gtir = 10,
	Gtri = 11,
	Gtrr = 12,
	Eqir = 13,
	Eqri = 14,
	Eqrr = 15
};

struct Register
{
	int vals[4] = {0, 0, 0, 0};
};

struct Instruction
{
	int id;
	int param1;
	int param2;
	int param3;
};

struct InstructionPosibility
{
	vector<Operations> possibleOperations;
};

ifstream fin("puzzle.in");
ofstream fout("puzzle.out");

InstructionPosibility instructions[16];

int result1 = 0;

Operations operationMap[16];
Operations solution[16];

vector<Instruction> secondPartInstructions;

void ParseRegister(Register& reg, char* line)
{
	int length = strlen(line);
	int regIndex = 0;
	bool prevDigit = false;

	for (int i = 0; i < length; i++)
	{
		if (line[i] >= '0' && line[i] <= '9')
		{
			prevDigit = true;
			reg.vals[regIndex] = (reg.vals[regIndex] * 10) + (line[i] - '0');
		}
		else
		{
			if (prevDigit)
			{
				prevDigit = false;
				regIndex++;
			}
		}
	}
}

void ApplyOperation(Register& reg, Operations operation, Instruction& parameters)
{
	switch (operation)
	{
	case Addr:
		reg.vals[parameters.param3] = reg.vals[parameters.param1] + reg.vals[parameters.param2];
		break;
	case Addi:
		reg.vals[parameters.param3] = reg.vals[parameters.param1] + parameters.param2;
		break;
	case Mulr:
		reg.vals[parameters.param3] = reg.vals[parameters.param1] * reg.vals[parameters.param2];
		break;
	case Muli:
		reg.vals[parameters.param3] = reg.vals[parameters.param1] * parameters.param2;
		break;
	case Banr:
		reg.vals[parameters.param3] = reg.vals[parameters.param1] & reg.vals[parameters.param2];
		break;
	case Bani:
		reg.vals[parameters.param3] = reg.vals[parameters.param1] & parameters.param2;
		break;
	case Borr:
		reg.vals[parameters.param3] = reg.vals[parameters.param1] | reg.vals[parameters.param2];
		break;
	case Bori:
		reg.vals[parameters.param3] = reg.vals[parameters.param1] | parameters.param2;
		break;
	case Setr:
		reg.vals[parameters.param3] = reg.vals[parameters.param1];
		break;
	case Seti:
		reg.vals[parameters.param3] = parameters.param1;
		break;
	case Gtir:
		reg.vals[parameters.param3] = ((parameters.param1 > reg.vals[parameters.param2]) ? 1 : 0);
		break;
	case Gtri:
		reg.vals[parameters.param3] = ((reg.vals[parameters.param1] > parameters.param2) ? 1 : 0);
		break;
	case Gtrr:
		reg.vals[parameters.param3] = ((reg.vals[parameters.param1] > reg.vals[parameters.param2]) ? 1 : 0);
		break;
	case Eqir:
		reg.vals[parameters.param3] = ((parameters.param1 == reg.vals[parameters.param2]) ? 1 : 0);
		break;
	case Eqri:
		reg.vals[parameters.param3] = ((reg.vals[parameters.param1] == parameters.param2) ? 1 : 0);
		break;
	case Eqrr:
		reg.vals[parameters.param3] = ((reg.vals[parameters.param1] == reg.vals[parameters.param2]) ? 1 : 0);
		break;
	}
}

int NumPossib(Register& reg1, Instruction& instruction1, Register& reg2)
{
	int numPossib = 0;
	for (int i = 0; i < 16; i++)
	{
		Register workingRegister = Register();
		memcpy(workingRegister.vals, reg1.vals, sizeof(int) * 4);

		ApplyOperation(workingRegister, (Operations)i, instruction1);

		bool goodOperation = true;

		for (int j = 0; j < 4; j++)
		{
			if (reg2.vals[j] != workingRegister.vals[j])
				goodOperation = false;
		}

		if (goodOperation)
		{
			instructions[instruction1.id].possibleOperations.push_back((Operations)i);
			numPossib++;
		}
	}

	return numPossib;
}

void Read()
{
	char line[MAX_LINE_LENGTH] = { NULL };

	while (fin.getline(line, MAX_LINE_LENGTH))
	{
		if (line[0] == 'B')
		{
			Register firstRegister = Register();
			ParseRegister(firstRegister, line);

			fin.getline(line, MAX_LINE_LENGTH);
			Instruction instruction = Instruction();
			sscanf(line, "%d %d %d %d", &instruction.id, &instruction.param1, &instruction.param2, &instruction.param3);

			fin.getline(line, MAX_LINE_LENGTH);
			Register secondRegister = Register();
			ParseRegister(secondRegister, line);

			int numPossib = NumPossib(firstRegister, instruction, secondRegister);

			if (numPossib >= 3)
			{
				result1++;
			}
		}

		if (line[0] >= '0' && line[0] <= '9')
		{
			Instruction instruction = Instruction();
			sscanf(line, "%d %d %d %d", &instruction.id, &instruction.param1, &instruction.param2, &instruction.param3);

			secondPartInstructions.push_back(instruction);
		}
	}
}

void IntersectSolutions()
{
	for (int i = 0; i < 16; i++)
	{
		int maxOccurence = 0;
		vector<Operations> resultedPossible;

		for (int j = 0; j < instructions[i].possibleOperations.size(); j++)
		{
			int occurence = 1;
			for (int k = j + 1; k < instructions[i].possibleOperations.size(); k++)
			{
				if (instructions[i].possibleOperations[k] == instructions[i].possibleOperations[j])
				{
					occurence++;
				}
			}

			if (occurence > maxOccurence)
			{
				maxOccurence = occurence;
				resultedPossible.clear();
				resultedPossible.push_back(instructions[i].possibleOperations[j]);
			}
			else
			{
				if (occurence == maxOccurence)
				{
					resultedPossible.push_back(instructions[i].possibleOperations[j]);
				}
			}
		}

		instructions[i].possibleOperations = resultedPossible;
	}
}

void Backtracking(int operation)
{
	if (operation >= 16)
	{
		memcpy(solution, operationMap, sizeof(int) * 16);
		cout << "If this message appears once, everything is allright... If it appears twice.. the algorithm is not complete\n";
	}
	else
	{
		for (int i = 0; i < instructions[operation].possibleOperations.size(); i++)
		{
			Operations possibleOperation = instructions[operation].possibleOperations[i];

			bool canContinue = true;

			for (int j = 0; j < operation; j++)
			{
				if (operationMap[j] == possibleOperation)
				{
					canContinue = false;
				}
			}

			if (canContinue)
			{
				operationMap[operation] = possibleOperation;
				Backtracking(operation + 1);
			}
		}
	}
}

void Solve2()
{
	IntersectSolutions();
	Backtracking(0);

	Register reg;

	reg.vals[0] = 0;
	reg.vals[1] = 0;
	reg.vals[2] = 0;
	reg.vals[3] = 0;

	for (int i = 0; i < secondPartInstructions.size(); i++)
	{
		ApplyOperation(reg, solution[secondPartInstructions[i].id], secondPartInstructions[i]);
	}

	fout << reg.vals[0] << endl;
}

int main(void)
{
	Read();

	fout << result1 << endl;

	Solve2();

	fin.close();
	fout.close();

	return 0;
}