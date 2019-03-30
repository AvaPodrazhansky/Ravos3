#include "pch.h"

CPU::CPU(bool AllocateMemory)
{
	if (AllocateMemory)
		m_Memory = new Memory(1024);
}

Instruction CPU::Decode(const MemoryWord &Word)
{
	// https://stackoverflow.com/questions/11452920/how-to-cast-int-to-enum-in-c
	//int n = Word.Instruction.OpCode;
	Instruction i = static_cast<Instruction>(Word.Instruction.OpCode);
	return i;
}

//Prints instruction as formatted instruction
void CPU::DumpMemoryAsInstructions(MemoryWord w, std::string instruc)
{
	printf("%2d %8s %3d %3d %8d \n", w.Instruction.InstType, 
		 instruc.c_str(), w.Instruction.Reg1, w.Instruction.Reg2,
		w.Instruction.GetAddress());
}


#define PreExecute(w,inst,assertpart)  \
if (printContents) { std::cout << std::hex << w.Contents; } \
if (printInstruction) { DumpMemoryAsInstructions(w, inst); } \
    w.assertpart(); \
	if (!isExecuting) break; 

#define ChangePC(address) m_PC = w.address() / 4 + m_PCB->StartIndexRAM;

bool CPU::Execute()
{
	if (printInstruction || printContents) std::cout << "************Printing Job " << m_PCB->process_ID << " **************"<<std::endl;
	m_C_State = BUSY;
	m_PCB->state = Running;

	do
	{
		MemoryWord w = m_Memory->read(m_PC, m_PCB->StartIndexRAM); //Fetch
		Instruction i = Decode(w);
		++m_PC;
		switch (i)
		{
			//Reads content of I/P buffer in m_Disk or m_Register into a accumulator
		case I_RD:
		{
			PreExecute(w, "I_RD", AssertInstructionTypeIO);

			if (w.Address16() != 0)
			{
				unsigned int offsetAddress = w.Address16() / 4;
//				m_Register[w.RegR1()] = m_Disk->readContents(offsetAddress - m_PCB->ProgramSize, m_PCB->StartIndexDisk); //will probably have to offset based on start location in disk
//				if (1) std::cout << "   -- Read at " << offsetAddress - m_PCB->ProgramSize << " (offset " << m_PCB->StartIndexDisk << ") value " << m_Register[w.RegR1()] << "\n";
				m_Register[w.RegR1()] = m_Disk->readContents(offsetAddress, m_PCB->StartIndexDisk); //will probably have to offset based on start location in disk
				if (1) std::cout << "   -- Read at " << offsetAddress << " (offset " << m_PCB->StartIndexDisk << ") value " << m_Register[w.RegR1()] << "\n";

//				if (printLog) std::cout << "m_Register[" << w.RegR1() << "] = m_Disk[" << offsetAddress - m_PCB->ProgramSize << " offset " << m_PCB->StartIndexDisk << "]\n";
			}
			else
			{
				//m_Register[w.RegR1()] = m_Register[w.RegR2()];
//				m_Register[w.RegR1()] = m_Disk->readContents( m_Register[w.RegR2()] / 4 - m_PCB->ProgramSize, m_PCB->StartIndexDisk);
//				if (1) std::cout << "   -- Read at " << m_Register[w.RegR2()] / 4 - m_PCB->ProgramSize << " (offset " << m_PCB->StartIndexDisk << ") value " << m_Register[w.RegR1()] << "\n";
				m_Register[w.RegR1()] = m_Disk->readContents(m_Register[w.RegR2()] / 4, m_PCB->StartIndexDisk);
				if (1) std::cout << "   -- Read at " << m_Register[w.RegR2()] / 4 << " (offset " << m_PCB->StartIndexDisk << ") value " << m_Register[w.RegR1()] << "\n";

			}
			break;
		}
		//Writes the content of accumulator into O/P buffer in m_Disk or m_Register
		case I_WR:
		{
			PreExecute(w, "I_WR", AssertInstructionTypeIO);

			if (w.Address16() != 0)
			{
				unsigned int offsetAddress = w.Address16() / 4;
//				m_Disk->write(offsetAddress - m_PCB->ProgramSize, (MemoryWord)m_Register[w.RegR1()], m_PCB->OutputBufferStart); //might possibly be reg1
//				if (1) std::cout << "   -- Write at " << offsetAddress - m_PCB->ProgramSize << " (offset " << m_PCB->OutputBufferStart << ") value " << m_Register[w.RegR1()] << "\n";
				MemoryWord temp = (MemoryWord)m_Register[w.RegR1()];
				m_Disk->write(offsetAddress, temp, m_PCB->StartIndexDisk); //might possibly be reg1
				if (1) std::cout << "   -- Write at " << offsetAddress << " (offset " << m_PCB->StartIndexDisk << ") value " << m_Register[w.RegR1()] << "\n";
				//Need to check if the conversion to memory word is correct
				//m_Disk->write(w.Address16(), (MemoryWord)m_Register[w.RegR1()]); //converts MemoryWord in buffer to int to be stored in m_Register
			}
			else
			{
				m_Register[w.RegR2()] = m_Register[w.RegR1()];
			}
			break;

		}
		//Stores content of a reg.  into an address
		case I_ST:
		{
			PreExecute(w, "I_ST", AssertInstructionTypeI);

			//m_Memory->write(m_Register[w.RegB()], m_Disk->readContents(m_Register[w.RegD()]/4 - m_PCB->ProgramSize));
			m_Memory->write(m_Register[w.RegD()], (MemoryWord) m_Register[w.RegB()], m_PCB->StartIndexRAM);
			//m_Memory->write(m_Register[w.RegD()] / 4 + m_PCB->StartIndexRAM, (MemoryWord)m_Register[w.RegB()]);
			break;
		}
		//Loads the content of an address into a reg
		case I_LW:
		{
			PreExecute(w, "I_LW", AssertInstructionTypeI);

			//m_Register[w.RegD()] = (m_Memory->readContents((m_Register[w.RegB()] + w.Address16() /4)));
			m_Register[w.RegD()] = m_Memory->readContents(m_Register[w.RegB()] / 4, m_PCB->StartIndexRAM);
			//m_Register[w.RegD()] = m_Memory->readContents(m_Register[w.RegB()] / 4 + m_PCB->StartIndexRAM);

			break;
		}
		//Transfers the content of one register into another
		case I_MOV:
		{
			PreExecute(w, "I_MOV", AssertInstructionTypeR);

			//m_Register[w.RegD()] = m_Register[w.RegS1()];
			m_Register[w.RegS1()] = m_Register[w.RegS2()];
			break;
		}
		//Adds content of two S-regs into D-reg
		case I_ADD:
		{
			PreExecute(w, "I_ADD", AssertInstructionTypeR);

			m_Register[w.RegD()] = m_Register[w.RegS1()] + m_Register[w.RegS2()];
			break;
		}
		//Subtracts content of two S-regs into D-reg
		case I_SUB:
		{
			PreExecute(w, "I_SUB", AssertInstructionTypeR);

			m_Register[w.RegD()] = m_Register[w.RegS1()] - m_Register[w.RegS2()];
			break;
		}
		//Multiplies content of two S-regs into D-reg
		case I_MUL:
		{
			PreExecute(w, "I_MUL", AssertInstructionTypeR);

			m_Register[w.RegD()] = m_Register[w.RegS1()] * m_Register[w.RegS2()];
			break;
		}
		//Divides content of two S-regs into D-reg
		case I_DIV:
		{
			PreExecute(w, "I_DIV", AssertInstructionTypeR);

			m_Register[w.RegD()] = m_Register[w.RegS1()] / m_Register[w.RegS2()];
			break;
		}
		//Logical AND of two S-regs into D-reg
		case I_AND:
		{
			PreExecute(w, "I_AND", AssertInstructionTypeR);

			m_Register[w.RegD()] = (m_Register[w.RegS1()] & m_Register[w.RegS2()]);
			break;
		}
		//Logical OR of two S-regs into D-reg
		case I_OR:
		{
			PreExecute(w, "I_OR", AssertInstructionTypeR);

			m_Register[w.RegD()] = (m_Register[w.RegS1()] | m_Register[w.RegS2()]);
			break;
		}
		//Transfers address/data directly into a register
		case I_MOVI:
		{
			PreExecute(w, "I_MOVI", AssertInstructionTypeI);

			m_Register[w.RegD()] = w.Address16();
			if(printLog) std::cout<< "MOVI val: "<< w.Address16() << "to R" << w.RegD() << "\n";
			break;
		}
		//Adds a data value directly to the content of a register
		case I_ADDI:
		{
			PreExecute(w, "I_ADDI", AssertInstructionTypeI);

			m_Register[w.RegD()] += w.Address16();// / 4; //could possibly not divide by 4
			break;
		}
		//Multiplies a data value directly with the content of a register
		case I_MULI:
		{
			PreExecute(w, "I_MULI", AssertInstructionTypeI);

			m_Register[w.RegD()] *= w.Address16();
			break;
		}
		//Divides a data directly to the content of a register
		case I_DIVI:
		{
			PreExecute(w, "I_DIVI", AssertInstructionTypeI);

			m_Register[w.RegD()] /= w.Address16();
			break;
		}
		//Loads a data/address directly to the content of a register
		case I_LDI:
		{
			PreExecute(w, "I_LDI", AssertInstructionTypeI);
			//May offset address here, may just do that in the read/write or st/lw funcitons so we know how much to offset. 
			//unsigned int offsetAddress = w.Address16() / 4;// -m_PCB->ProgramSize + m_PCB->StartIndexRAM;
			//if (offsetAddress < m_PCB->ProgramSize) offsetAddress -= m_PCB->ProgramSize;
			m_Register[w.RegD()] = w.Address16(); //might need to add offset
			break;
		}
		//Sets the D-reg to 1 if first S-reg is less than the B-reg; 0 otherwise
		case I_SLT:
		{
			PreExecute(w, "I_SLT", AssertInstructionTypeR);

			if (m_Register[w.RegS1()] < m_Register[w.RegS2()])
				m_Register[w.RegD()] = 1;
			else
				m_Register[w.RegD()] = 0;
			break;
		}
		//Sets the D-reg to 1 if  first S-reg is less than a data; 0 otherwise
		case I_SLTI:
		{
			PreExecute(w, "I_SLTI", AssertInstructionTypeI);

			if (m_Register[w.RegB()] < w.Address16())
				m_Register[w.RegD()] = 1;
			else
				m_Register[w.RegD()] = 0;
			break;
		}
		//Logical end of program
		case I_HLT:
		{
			PreExecute(w, "I_HLT", AssertInstructionTypeJ);
			//if (!isExecuting) return true;
			//clear registers
			for (int x : m_Register)
				m_Register[x] = 0;

			//m_PC = NULL //dispatcher will set CPU's m_PC to the next PCB's program counter
			m_PC = NULL;
			//signal scheduler

			//Terminate PCB
			m_PCB->state = Terminated;
			
			//Set CPU back to idle (for scheduling)
			m_C_State = IDLE;
			return true;
			break;
		}
		//Does nothing and moves to next instruction
		case I_NOP:
		{
			std::cout << " I_NOP"; // << I_NOP << std::endl;
			if (printInstruction) { DumpMemoryAsInstructions(w, "I_NOP"); }
			if (printContents) { std::cout << std::hex << w.Contents; }
			if (!isExecuting) break;

			//m_PC++;
			break;
		}
		//Jumps to a specified location
		case I_JMP:
		{
			PreExecute(w, "I_JMP", AssertInstructionTypeJ);

			//m_PC = w.Address24(); //might need to divide by 4
			ChangePC(Address24);
			break;
		}
		//Branches to an address when content of B-reg = D-reg
		case I_BEQ:
		{
			PreExecute(w, "I_BEQ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] == m_Register[w.RegD()])
			{
				std::cout << "Loop\n";
				ChangePC(Address16);
			}
			else std::cout << "End Loop\n";
			break;
		}
		//Branches to an address when content of B-reg <> D-reg
		case I_BNE:
		{
			PreExecute(w, "I_BNE", AssertInstructionTypeI);

			if (m_Register[w.RegB()] != m_Register[w.RegD()])
			{
				std::cout << "Loop\n";
				ChangePC(Address16);
			}
			else std::cout << "End Loop\n";
			break;
		}
		//Branches to an address when content of B-reg = 0
		case I_BEZ:
		{
			PreExecute(w, "I_BEZ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] == 0)
			{
				std::cout << "Loop\n";
				ChangePC(Address16);
			}
			else std::cout << "End Loop\n";
			break;
		}
		//Branches to an address when content of B-reg != 0
		case I_BNZ:
		{
			PreExecute(w, "I_BNZ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] != 0)
			{
				std::cout << "Loop\n";
				ChangePC(Address16);
			}
			else std::cout << "End Loop\n";
			break;
		}
		//Branches to an address when content of B-reg > 0
		case I_BGZ:
		{
			PreExecute(w, "I_BGZ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] > 0)
			{
				std::cout << "Loop\n";
				ChangePC(Address16);
			}
			else std::cout << "End Loop\n";
		}
		//Branches to an address when content of B-reg < 0
		case I_BLZ:
		{
			PreExecute(w, "I_BLZ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] < 0)
			{
				std::cout << "Loop\n";
				ChangePC(Address16);
			}
			else std::cout << "End Loop\n";
			break;
		}

		default:
		{
			std::cout << "didn't work: " << w.Contents; // << std::endl;
			assert(0); // instruction does not exist
		}

		}
	}while (m_PCB->state != Terminated);
	
	return false;
}
