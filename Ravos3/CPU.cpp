#include "pch.h"

CPU::CPU(bool AllocateMemory)
{
	if (AllocateMemory)
		m_Memory = new Memory(1024);
}


bool CPU::MapPageIntoMemory(int ProcessID, int Page)
{
	return false;
}

bool CPU::assignPCB(PCB* pcb)
{
	m_PCB = pcb;
	m_PC = pcb->programCounter;

	return true;
}

void CPU::CPU_Run_thread()
{
	    m_C_State = BUSY;

		Execute();

		m_C_State = IDLE;

}


void CPU::FlushBuffers()
{
	if (!m_MMU->ManageMemory)
	{
		for (int i = m_PCB->getProgramSize() + m_PCB->InputBufferSize; i < m_PCB->getProgramSize() + m_PCB->InputBufferSize + m_PCB->TempBufferSize + m_PCB->OutputBufferSize; i++)
		{
			MemoryWord w = m_Memory->readContents(i, m_PCB->getStartIndexRAM());
			m_Disk->write(i, w, m_PCB->getStartIndexDisk()); // Write instructions and buffers back to disk
		}
		m_MMU->ClearWholeProcessFromRAM(m_PCB);
		return;
	}
		
	//m_MMU->printValidFrames();
	m_MMU->RemovePagesFromRAM(m_PCB);

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

#define ChangePC(address) m_PC = w.address() / 4 ;

bool CPU::Execute()
{
	if (printInstruction || printContents) std::cout << "************Printing Job " << m_PCB->process_ID << " **************" << std::endl;
	//if (testSchededuler) printf("%5d %5d %9d %9d\n", CPU_ID, m_PCB->process_ID, m_PCB->getPriority(), m_PCB->getProgramSize());
	//m_C_State = BUSY;
	m_PCB->state = Running;

	auto start = std::chrono::high_resolution_clock::now();//start time of process
	do
	{
		MemoryWord w = m_MMU->ReadOrPageFault(m_PC, m_PCB); // Fetch
	
		Instruction i = Decode(w);
		try
		{
			++m_PC;
			switch (i)
			{
				//Reads content of I/P buffer in m_Disk or m_Register into a accumulator
				case I_RD:
				{
					PreExecute(w, "I_RD", AssertInstructionTypeIO);
					{
						if (w.Address16() != 0)
						{							
							m_Register[w.RegR1()] = m_MMU->ReadOrPageFault(w.Address16() / 4, m_PCB).Contents;
							
						}
						else
						{
							m_Register[w.RegR1()] = m_MMU->ReadOrPageFault(m_Register[w.RegR2()] / 4, m_PCB).Contents;
						
						}
						
						m_PCB->updateIOCount();
						break;
					}
				}
				//Writes the content of accumulator into O/P buffer in m_Disk or m_Register
				case I_WR:
				{
					PreExecute(w, "I_WR", AssertInstructionTypeIO);

					if (w.Address16() != 0)
					{
					
						m_MMU->WriteOrPageFault(w.Address16() / 4, (MemoryWord)m_Register[w.RegR1()], m_PCB);
															
					}
					else
					{
						m_MMU->WriteOrPageFault(m_Register[w.RegR2()] / 4, (MemoryWord)m_Register[w.RegR1()], m_PCB);
					}
					m_PCB->updateIOCount();
					break;

				}
				//Stores content of a reg.  into an address
				case I_ST:
				{
					PreExecute(w, "I_ST", AssertInstructionTypeI);

					m_MMU->WriteOrPageFault(m_Register[w.RegD()] / 4, (MemoryWord)m_Register[w.RegB()], m_PCB);
					m_PCB->updateIOCount();
					break;
				}
				//Loads the content of an address into a reg
				case I_LW:
				{
					PreExecute(w, "I_LW", AssertInstructionTypeI);
					m_Register[w.RegD()] = (m_MMU->ReadOrPageFault((m_Register[w.RegB()] + w.Address16()) / 4, m_PCB)).Contents;
					m_PCB->updateIOCount();
					break;
				}
				//Transfers the content of one register into another
				case I_MOV:
				{
					PreExecute(w, "I_MOV", AssertInstructionTypeR);

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
					if (printLog) std::cout << "MOVI val: " << w.Address16() << "to R" << w.RegD() << "\n";
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

					for (unsigned int i = 0; i < MAX_REGISTERS; i++)
					{
						m_Register[i] = 0;
					}

					m_PCB->programCounter = m_PC; 
					m_PC = NULL;

					FlushBuffers();

					m_PCB->state = Terminated;


					auto stop = std::chrono::high_resolution_clock::now();//end time of process
					auto completionTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

					m_PCB->setCompletionTime(completionTime.count());//set difference as completion time

					m_PCB = NULL;

					return true;
				}
				//Does nothing and moves to next instruction
				case I_NOP:
				{
					std::cout << " I_NOP"; // << I_NOP << std::endl;
					if (printInstruction) { DumpMemoryAsInstructions(w, "I_NOP"); }
					if (printContents) { std::cout << std::hex << w.Contents; }
					if (!isExecuting) break;

					break;
				}
				//Jumps to a specified location
				case I_JMP:
				{
					PreExecute(w, "I_JMP", AssertInstructionTypeJ);

					ChangePC(Address24);
					break;
				}
				//Branches to an address when content of B-reg = D-reg
				case I_BEQ:
				{
					PreExecute(w, "I_BEQ", AssertInstructionTypeI);

					if (m_Register[w.RegB()] == m_Register[w.RegD()])
					{
						ChangePC(Address16);
					}
					break;
				}
				//Branches to an address when content of B-reg <> D-reg
				case I_BNE:
				{
					PreExecute(w, "I_BNE", AssertInstructionTypeI);

					if (m_Register[w.RegB()] != m_Register[w.RegD()])
					{
						ChangePC(Address16);
					}
					break;
				}
				//Branches to an address when content of B-reg = 0
				case I_BEZ:
				{
					PreExecute(w, "I_BEZ", AssertInstructionTypeI);

					if (m_Register[w.RegB()] == 0)
					{
						ChangePC(Address16);
					}
					break;
				}
				//Branches to an address when content of B-reg != 0
				case I_BNZ:
				{
					PreExecute(w, "I_BNZ", AssertInstructionTypeI);

					if (m_Register[w.RegB()] != 0)
					{
						ChangePC(Address16);
					}
					break;
				}
				//Branches to an address when content of B-reg > 0
				case I_BGZ:
				{
					PreExecute(w, "I_BGZ", AssertInstructionTypeI);

					if (m_Register[w.RegB()] > 0)
					{
						ChangePC(Address16);
					}
				}
				//Branches to an address when content of B-reg < 0
				case I_BLZ:
				{
					PreExecute(w, "I_BLZ", AssertInstructionTypeI);

					if (m_Register[w.RegB()] < 0)
					{
						ChangePC(Address16);
					}
					break;
				}

				default:
				{
					std::cout << "didn't work: " << w.Contents; // << std::endl;
					assert(0); // instruction does not exist
				}

			}
		}
		catch (RavosInvalidMemoryAddressException &e)
		{
			std::cout << "\nFAULT: Process " << e.ProcessID << " accesed inavalid memory on page " << e.Page << " (PC=" << e.PC << ") - TERMINATED\n";
			m_PCB->state = Terminated;
		}
		catch (RavosMemoryProtectionException &e)
		{
			std::cout << "\nFAULT: Process " << e.ProcessID << " accesed memory that it does not own on page " << e.Page << " (PC=" << e.PC << ") - TERMINATED\n";
			m_PCB->state = Terminated;
		}
		catch (RavosPagingFaultException &e)
		{

			// Process loading of new page into memory
			m_PC = e.PC;  // Set the PC to the instruction executing when the page fault occurred
			if (!MapPageIntoMemory(e.ProcessID, e.Page))
			{
				// Process will be added to queue of processes awaiting a page
				break;
			}
			
			// Allow the instruction to restart on next iteration of loop
		}
		catch (std::exception e)
		{
			std::cout << "\nFAULT: Process " << m_PCB->getProcessID() << " had an unidentified fault (PC=~" << m_PC << ") - TERMINATED\n";
			m_PCB->state = Terminated;
		}
	} while (m_PCB->state != Terminated);

	return false;
}
