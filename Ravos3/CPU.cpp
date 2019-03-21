#include "pch.h"

CPU::CPU(bool AllocateMemory)
{
	if (AllocateMemory)
		m_Memory = new Memory(1024);
}

Instruction CPU::Decode(const MemoryWord &Word)
{
	// https://stackoverflow.com/questions/11452920/how-to-cast-int-to-enum-in-c
	int n = Word.Instruction.OpCode;
	Instruction i = static_cast<Instruction>(n);
	return i;
}

void CPU::DumpMemoryAsInstructions(MemoryWord w, std::string instruc)
{
	//int End = Start + Length;
	//for (int i = Start; i < End; ++i)
	//{
	//	//MemoryWord w = m_Memory->read(i);
	//	//printf("%3d ", i);
	//	//printf("%d ", w.InstuctionType());
	//	//printf("%2d ", w.HexToInt(0, 2) & 0x3f);


	//	//printf("\n");

	//}
	//std::cout << w.Contents << std::endl;
	//std::cout << w.Instruction.InstType << " ";
	////std::cout << execute(w) << " ";
	////execute(w);
	//std::cout << " " << w.Instruction.Reg1 << " ";
	//std::cout << w.Instruction.Reg2 << " ";
	//std::cout << w.Instruction.GetAddress() << std::endl;
	printf("%2d %8d %3 %3d %8d \n", w.Instruction.InstType, instruc, w.Instruction.Reg1, w.Instruction.Reg2, w.Instruction.GetAddress());

}

//void CPU::ExecuteIt(MemoryWord w, std::string inst /*, pointer to instruction's equation*/)
//{
//	//Print formatted instruction if true
//	if (printInstruction) { DumpMemoryAsInstructions(w, inst); }
//
//	//Print contents if true
//	if (printContents) { std::cout << w.Contents; }
//
//	//Execute if true
//	if(!isExecuting) {/*pointer to equation to be executed*/ }
//}



void CPU::Execute()
{
	MemoryWord w = m_Memory->read(m_PC); //Fetch
	Instruction i = Decode(w);
	++m_PC;
	switch (i)
	{
			//Reads content of I/P buffer into a accumulator
		case I_RD:
		{
			//std::cout << " " << "I_RD";// << std::endl;
			if (printInstruction) DumpMemoryAsInstructions(w, "I_RD");
			w.AssertInstructionTypeIO();
			if (!isExecuting)break;
			if (w.Address16() != 0)// if address field is non-zero, read from buffers in m_Disk
			{
				//m_Register[w.RegR1()] = m_Disk.read(w.Address16()); //converts MemoryWord in buffer to int to be stored in m_Register
			}
			else // if address field is zero, read from m_Register
			{
				m_Register[w.RegR1()] = m_Register[w.RegR2()];
			}
			break;
		}
		//Writes the content of accumulator into O/P buffer
		case I_WR:
		{
			std::cout << " " << "I_WR";// << std::endl;
			if (printInstruction) DumpMemoryAsInstructions(w, "I_RD");
			
			w.AssertInstructionTypeIO();
			
			if (!isExecuting)break;
			if (w.Address16() != 0)// if address field is non-zero, write to buffers in m_Disk
			{
				//m_Disk.write(w.Address16(), m_Register[w.RegR1()]); //converts MemoryWord in buffer to int to be stored in m_Register
			}
			else // if address field is zero, write to m_Register
			{
				m_Register[w.RegR2()] = m_Register[w.RegR1()];
			}
			break;

			break;
		}
		//Stores content of a reg.  into an address
		case I_ST:
		{
			std::cout << " I_ST"; // << I_ST << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			//WORK IN PROGRESS
			//w.Address16() = m_Register[w.RegD()];
			//write(w.Address16() + m_Register[w.RegR2()],m_Register[w.RegR1()])
			break;
			//Loads the content of an address into a reg
		}
		case I_LW:
		{
			//OFFSET MUST BE INITIALIZED (in MMU) but for now is zero (we need to put offset in PCB)
			std::cout << " I_LW"; // << I_LW << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			//WORK IN PROGRESS
			//m_Register[w.RegD()] = w.Address16();
			// m_Register[w.RegR1()] = Decode(m_Memory->read(w.Address16() + m_Register[w.RegR2()]));
			break;
		}
		//Transfers the content of one register into another
		case I_MOV:
		{
			std::cout << " I_MOV";// << I_MOV << std::endl;
			w.AssertInstructionTypeR();
			if (!isExecuting)break;
			m_Register[w.RegD()] = m_Register[w.RegS1()];
			break;
		}
		//Adds content of two S-regs into D-reg
		case I_ADD:
		{
			std::cout << " I_ADD";// << I_ADD << std::endl;
			w.AssertInstructionTypeR();
			if (!isExecuting)break;
			m_Register[w.RegD()] = m_Register[w.RegS1()] + m_Register[w.RegS2()];
			break;
		}
		//Subtracts content of two S-regs into D-reg
		case I_SUB:
		{
			std::cout << " I_SUB";// << I_SUB << std::endl;
			w.AssertInstructionTypeR();
			if (!isExecuting)break;
			m_Register[w.RegD()] = m_Register[w.RegS1()] - m_Register[w.RegS2()];
			break;
		}
		//Multiplies content of two S-regs into D-reg
		case I_MUL:
		{
			std::cout << " I_MUL";// << I_MUL << std::endl;
			w.AssertInstructionTypeR();
			if (!isExecuting)break;
			m_Register[w.RegD()] = m_Register[w.RegS1()] * m_Register[w.RegS2()];
			break;
		}
		//Divides content of two S-regs into D-reg
		case I_DIV:
		{
			std::cout << " I_DIV"; // << I_DIV << std::endl;
			w.AssertInstructionTypeR();
			if (!isExecuting)break;
			m_Register[w.RegD()] = m_Register[w.RegS1()] / m_Register[w.RegS2()];
			break;
		}
		//Logical AND of two S-regs into D-reg
		case I_AND:
		{
			std::cout << " I_AND";//  << I_AND << std::endl;
			w.AssertInstructionTypeR();
			if (!isExecuting)break;
			m_Register[w.RegD()] = (m_Register[w.RegS1()] & m_Register[w.RegS2()]);
			break;
		}
		//Logical OR of two S-regs into D-reg
		case I_OR:
		{
			std::cout << " I_OR";// << I_OR << std::endl;
			w.AssertInstructionTypeR();
			if (!isExecuting)break;
			m_Register[w.RegD()] = (m_Register[w.RegS1()] | m_Register[w.RegS2()]);
			break;
		}
		//Transfers address/data directly into a register
		case I_MOVI:
		{
			std::cout << " " << "I_MOVI";// << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			m_Register[w.RegD()] = w.Address16();
			break;
		}
		//Adds a data value directly to the content of a register
		case I_ADDI:
		{
			std::cout << " I_ADDI";// << I_ADDI << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			//address16 is being used as the data "The Address may at times be treated as data, which is direct addressing"
			m_Register[w.RegD()] = w.Address16() + m_Register[w.RegB()];
			break;
		}
		//Multiplies a data value directly with the content of a register
		case I_MULI:
		{
			std::cout << " I_MULI"; // << I_MULI << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			//address16 is being used as the data "The Address may at times be treated as data, which is direct addressing"
			m_Register[w.RegD()] = w.Address16() * m_Register[w.RegB()];
			break;
		}
		//Divides a data directly to the content of a register
		case I_DIVI:
		{
			std::cout << " I_DIVI"; // << I_DIVI << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			m_Register[w.RegD()] /= w.Address16();//
			break;
		}
		//Loads a data/address directly to the content of a register
		case I_LDI:
		{
			std::cout << " I_LDI"; //  << I_LDI << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			//address16 is being used as the data "The Address may at times be treated as data, which is direct addressing"
			m_Register[w.RegD()] = w.Address16();
			break;
		}
		//Sets the D-reg to 1 if  first S-reg is less than the B-reg; 0 otherwise
		case I_SLT:
		{
			std::cout << " I_SLT"; //  << I_SLT << std::endl;
			w.AssertInstructionTypeR();
			if (!isExecuting)break;
			if (m_Register[w.RegS1()] < m_Register[w.RegS2()])
				m_Register[w.RegD()] = 1;
			else
				m_Register[w.RegD()] = 0;
			break;
		}
		//Sets the D-reg to 1 if  first S-reg is less than a data; 0 otherwise
		case I_SLTI:
		{
			std::cout << " I_SLTI"; // I_SLTI << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			if (m_Register[w.RegS1()] < w.Address16())
				m_Register[w.RegD()] = 1;
			else
				m_Register[w.RegD()] = 0;
			break;
		}
		//Logical end of program
		case I_HLT:
		{
			std::cout << " I_HLT"; // << I_HLT << std::endl;
			w.AssertInstructionTypeJ();
			if (!isExecuting)break;
			//clear registers
			//he has program counter = job counter
			break;
		}
		//Does nothing and moves to next instruction
		case I_NOP:
		{
			std::cout << " I_NOP"; // << I_NOP << std::endl;
			break;
		}
		//Jumps to a specified location
		case I_JMP:
		{
			std::cout << " I_JMP"; // << I_JMP << std::endl;
			w.AssertInstructionTypeJ();
			if (!isExecuting)break;
			//m_PC = w.Address24();
			break;
		}
		//Branches to an address when content of B-reg = D-reg
		case I_BEQ:
		{
			std::cout << " I_BEQ"; // << I_BEQ << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			if (m_Register[w.RegB()] == m_Register[w.RegD()])
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg <> D-reg
		case I_BNE:
		{
			std::cout << " I_BNE"; // << I_BNE << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			if (m_Register[w.RegB()] != m_Register[w.RegD()])
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg = 0
		case I_BEZ:
		{
			std::cout << " I_BEZ"; //  << I_BEZ << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			if (m_Register[w.RegB()] == 0)
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg <> 0
		case I_BNZ:
		{
			std::cout << " I_BNZ"; // << I_BNZ << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			if (m_Register[w.RegB()] != 0)
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg > 0
		case I_BGZ:
		{
			std::cout << " I_BGZ"; // << I_BGZ << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
			if (m_Register[w.RegB()] > 0)
				m_PC = m_Register[w.Address16()];
			break;
		}
		//Branches to an address when content of B-reg < 0
		case I_BLZ:
		{
			std::cout << " I_BLZ"; // << I_BLZ << std::endl;
			w.AssertInstructionTypeI();
			if (!isExecuting)break;
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
