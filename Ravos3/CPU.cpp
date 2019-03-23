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
	printf("%2d %8d %3 %3d %8d \n", w.Instruction.InstType, 
		instruc, w.Instruction.Reg1, w.Instruction.Reg2, 
		w.Instruction.GetAddress());
}


#define PreExecute(w,inst,assertpart)  \
	if (printInstruction) { DumpMemoryAsInstructions(w, inst); } \
    w.assertpart(); \
	if (printContents) { std::cout << std::hex << w.Contents; } \
	if (!isExecuting) break; 


void CPU::Execute()
{
	MemoryWord w = m_Memory->read(m_PC); //Fetch
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
				m_Register[w.RegR1()] = m_Disk->readContents(w.Address16()); //converts MemoryWord in buffer to int to be stored in m_Register
			}
			else
			{
				m_Register[w.RegR1()] = m_Register[w.RegR2()];
			}
			break;
		}
		//Writes the content of accumulator into O/P buffer in m_Disk or m_Register
		case I_WR:
		{
			PreExecute(w, "I_WR", AssertInstructionTypeIO);

			if (w.Address16() != 0)
			{
				//Need to check if the conversion to memory word is correct
				m_Disk->write(w.Address16(), (MemoryWord) m_Register[w.RegR1()]); //converts MemoryWord in buffer to int to be stored in m_Register
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

			/* NEED INSTRUCTIONS HERE */
			break;
		}
		//Loads the content of an address into a reg
		case I_LW:
		{
			PreExecute(w, "I_LW", AssertInstructionTypeI);
			/* NEED INSTRUCTIONS HERE */
			break;
		}
		//Transfers the content of one register into another
		case I_MOV:
		{
			PreExecute(w, "I_MOV", AssertInstructionTypeR);

			m_Register[w.RegD()] = m_Register[w.RegS1()];
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
			break;
		}
		//Adds a data value directly to the content of a register
		case I_ADDI:
		{
			PreExecute(w, "I_ADDI", AssertInstructionTypeI);

			//address16 is being used as the data "The Address may at times be treated as data, which is direct addressing"
			m_Register[w.RegD()] = w.Address16() + m_Register[w.RegB()];
			break;
		}
		//Multiplies a data value directly with the content of a register
		case I_MULI:
		{
			PreExecute(w, "I_MULI", AssertInstructionTypeI);

			//address16 is being used as the data "The Address may at times be treated as data, which is direct addressing"
			m_Register[w.RegD()] = w.Address16() * m_Register[w.RegB()];
			break;
		}
		//Divides a data directly to the content of a register
		case I_DIVI:
		{
			PreExecute(w, "I_DIVI", AssertInstructionTypeI);

			m_Register[w.RegD()] /= w.Address16();//
			break;
		}
		//Loads a data/address directly to the content of a register
		case I_LDI:
		{
			PreExecute(w, "I_LDI", AssertInstructionTypeI);

			//address16 is being used as the data "The Address may at times be treated as data, which is direct addressing"
			m_Register[w.RegD()] = w.Address16();
			break;
		}
		//Sets the D-reg to 1 if  first S-reg is less than the B-reg; 0 otherwise
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

			if (m_Register[w.RegS1()] < w.Address16())
				m_Register[w.RegD()] = 1;
			else
				m_Register[w.RegD()] = 0;
			break;
		}
		//Logical end of program
		case I_HLT:
		{
			PreExecute(w, "I_HLT", AssertInstructionTypeJ);
			//clear registers
			//set PCB to terminated
			//m_PC = NULL //dispatcher will set CPU's m_PC to the next PCB's program counter
			//signal scheduler
			break;
		}
		//Does nothing and moves to next instruction
		case I_NOP:
		{
			std::cout << " I_NOP"; // << I_NOP << std::endl;
			if (printInstruction) { DumpMemoryAsInstructions(w, "I_NOP"); } 
			if (printContents) { std::cout << w.Contents; } 
			if (!isExecuting) break;

			m_PC++;
			break;
		}
		//Jumps to a specified location
		case I_JMP:
		{
			PreExecute(w, "I_JMP", AssertInstructionTypeJ);

			m_PC = w.Address24();
			break;
		}
		//Branches to an address when content of B-reg = D-reg
		case I_BEQ:
		{
			PreExecute(w, "I_BEQ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] == m_Register[w.RegD()])
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg <> D-reg
		case I_BNE:
		{
			PreExecute(w, "I_BNE", AssertInstructionTypeI);

			if (m_Register[w.RegB()] != m_Register[w.RegD()])
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg = 0
		case I_BEZ:
		{
			PreExecute(w, "I_BEZ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] == 0)
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg <> 0
		case I_BNZ:
		{
			PreExecute(w, "I_BNZ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] != 0)
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg > 0
		case I_BGZ:
		{
			PreExecute(w, "I_BGZ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] > 0)
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg < 0
		case I_BLZ:
		{
			PreExecute(w, "I_BLZ", AssertInstructionTypeI);

			if (m_Register[w.RegB()] < 0)
				m_PC = m_Register[w.Address16()];
			break;
		}

		default:
		{
			std::cout << "didn't work: " << w.Contents; // << std::endl;
			assert(0); // instruction does not exist
		}

	}
}
