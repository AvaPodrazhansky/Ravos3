#pragma once
#include "pch.h"

#define MAX_REGISTERS 16
#define MAX_INSTRUCTION_LENGTH 28
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

	int CPU_ID;
	std::thread *m_thread_ptr = NULL;

	static Instruction Decode(const MemoryWord &Word);

	bool Execute();

	int m_PC = 0;  // Program Counter
	unsigned int m_Register[MAX_REGISTERS];

	Memory *m_Memory = NULL;
	Memory *m_Disk = NULL;
	PCB *m_PCB = NULL;
	MMU * m_MMU = NULL;
	CPUMetrics *m_CPUMetrics = NULL;

	MemoryWord m_Cache[MAX_INSTRUCTION_LENGTH];
	
	volatile CPU_State m_C_State = IDLE;

	CPU_State GetState() { return m_C_State; }

	bool assignPCB(PCB* pcb);
	
	//std::mutex lock; //lock so CPU can wait to be assigned to job
	///*volatile bool m_assignedToJob = false;*/
	//std::condition_variable cv;
	
	bool MapPageIntoMemory(int ProcessID, int Page);

	//static volatile bool all_jobs_done;

	void CPU_Run_thread();

	void FlushBuffers();

	//Variables for Testing
	bool isExecuting = true;  //Executes instructions if true
	bool printInstruction = false; //Prints formatted instruction if true
	bool printContents = false; //Prints hex value of instruction if true
	bool printLog = false;
	bool testSchededuler = true;

	void DumpMemoryAsInstructions(MemoryWord w, std::string i);


};
