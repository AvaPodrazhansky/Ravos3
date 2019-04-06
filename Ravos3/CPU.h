#pragma once
#include "pch.h"

#define MAX_REGISTERS 16

enum Instruction {
	I_RD = 0x00, I_WR = 0x01, I_ST = 0x02, I_LW = 0x03,
	I_MOV = 0x04, I_ADD = 0x05, I_SUB = 0x06, I_MUL = 0x07, I_DIV = 0x08, I_AND = 0x09, I_OR = 0x0A,
	I_MOVI = 0x0B, I_ADDI = 0x0C, I_MULI = 0x0D, I_DIVI = 0x0E, I_LDI = 0x0F,
	I_SLT = 0X10, I_SLTI = 0x11, I_HLT = 0x12, I_NOP = 0x13, I_JMP = 0x14,
	I_BEQ = 0x15, I_BNE = 0x16, I_BEZ = 0x17, I_BNZ = 0x18, I_BGZ = 0x19, I_BLZ = 0x1A
};

enum CPU_State {IDLE = 1, BUSY = 2};

class CPU
{
public:
	CPU(bool AllocateMemory = false); //constructor

	static Instruction Decode(const MemoryWord &Word);

	bool Execute();

	int m_PC = 0;  // Program Counter
	unsigned int m_Register[MAX_REGISTERS];

	Memory *m_Memory = NULL;
	Memory *m_Disk = NULL;
	PCB *m_PCB = NULL;
	CPUMetrics *m_CPUMetrics = NULL;
	Memory *Cache[28];//has not been implemented

	
	CPU_State m_C_State = IDLE;

	//Variables for Testing
	bool isExecuting = true;  //Executes instructions if true
	bool printInstruction = false; //Prints formatted instruction if true
	bool printContents = false; //Prints hex value of instruction if true
	bool printLog = false;
	bool testSchededuler = true;

	void DumpMemoryAsInstructions(MemoryWord w, std::string i);


};
