#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#define MAX_LINE_LENGTH        1000
#define MAX_INSTRUCTION_LENGTH 10
#define NUM_REGISTERS          6

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

struct Instruction
{
	Operations operation;

	unsigned long long param1;
	unsigned long long param2;
	unsigned long long param3;
};

struct Register
{
	unsigned long long vals[NUM_REGISTERS] = { 0 };
};

ifstream fin("puzzle.in");
ofstream fout("puzzle.out");

char instructionStrings[][MAX_INSTRUCTION_LENGTH] =
{
	"addr",
	"addi",
	"mulr",
	"muli",
	"banr",
	"bani",
	"borr",
	"bori",
	"setr",
	"seti",
	"gtir",
	"gtri",
	"gtrr",
	"eqir",
	"eqri",
	"eqrr"
};

int instructionPointerIndex = 0;
vector<Instruction> instructions;
map<int, bool> gotRegister;

Register registers;
int prevValue = 0;
int firstValue = -1;

void Read()
{
	char line[MAX_LINE_LENGTH] = { NULL };

	while (fin.getline(line, MAX_LINE_LENGTH))
	{
		if (line[0] == '#')
		{
			sscanf(line, "#ip %d", &instructionPointerIndex);
		}
		else
		{
			char instruction[MAX_INSTRUCTION_LENGTH] = { NULL };
			Instruction actualInstruction;
			int actualInstructionIndex = -1;

			sscanf(line, "%s %llu %llu %llu", instruction, &actualInstruction.param1, &actualInstruction.param2, &actualInstruction.param3);

			for (int i = 0; i < 16; i++)
			{
				if (strcmp(instruction, instructionStrings[i]) == 0)
				{
					actualInstructionIndex = i;
				}
			}

			if (actualInstructionIndex != -1)
			{
				actualInstruction.operation = (Operations)actualInstructionIndex;
			}
			else
			{
				cout << instruction << endl;
				cout << "Unknown instruction\n";
			}

			instructions.push_back(actualInstruction);
		}
	}
}

void ApplyOperation(Register& reg, Instruction& parameters)
{
	int prevFirst = reg.vals[0];

	switch (parameters.operation)
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

	if (parameters.operation == Operations::Eqrr && parameters.param1 == 3 && parameters.param2 == 0 && parameters.param3 == 1)
	{
		if (firstValue == -1)
		{
			firstValue = registers.vals[3];
			fout << firstValue << endl;
		}

		if (gotRegister.find(registers.vals[3]) == gotRegister.end())
		{
			gotRegister[registers.vals[3]] = true;
			prevValue = registers.vals[3];
		}
		else
		{
			fout << prevValue << endl;
			registers.vals[instructionPointerIndex] = instructions.size() - 1;
		}
	}
}

void Solve()
{
	int instructionIndex = registers.vals[instructionPointerIndex];

	while (instructionIndex >= 0 && instructionIndex < instructions.size())
	{
		ApplyOperation(registers, instructions[instructionIndex]);

		registers.vals[instructionPointerIndex]++;
		instructionIndex = registers.vals[instructionPointerIndex];
	}
}

int main(void)
{
	Read();
	Solve();

	fin.close();
	fout.close();

	return 0;
}