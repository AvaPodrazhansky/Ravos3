#include "pch.h"

//PCB* Scheduler::GetPCBBlockAsArray()
//{
//
//	PCB *pcb = calloc(theOS->m_PCB_Map, sizeof(PCB));
//	return pcb;
//
//}



// Scheduler implementation
//We will have this return false if we are out of RAM (or if there is another problem loading into RAM)
//This is part of the short term scheudler (I think)
//bool Scheduler::Dispatch(PCB *pcb)
//{
//	//copy process from ReadyQueue from disk to RAM (if that funciton returns true, then switch CPU's pointers. Then return true)
//	if(WriteNewProcessToRAM(pcb));
//	theOS->m_Computer->m_CPU[0].m_PCB = pcb;
//	theOS->m_Computer->m_CPU[0].m_PC = 0; // m_Computer->m_CPU[0].m_PCB->StartIndexRAM;
//	return theOS->m_Computer->m_CPU[0].Execute();//this will be moved to the short term scheduler
//}

int Scheduler::FIFOScheduler()
{
//	std::priority_queue<PCB*> q;

	for (int i = 0; i < theOS->m_PCB_Map.size(); ++i)
	{
		PCB* pcb = theOS->m_PCB_Map[i];
		if (pcb->isExecuting())
		{
			m_JobQueue.push(pcb->getProcessID());
			//theOS->m_ShortTerm.Dispatch(pcb);
			//if(Dispatch(pcb)); //If Dispatch(pcb) = signal OS to execute
			//if all CPUs are busy, wait
		}
	}

	
	return 0;
}

int Scheduler::SJFScheduler()
{


	// TODO: Return the PID of process with the least amount of instructions. 
	return 0;
}

int Scheduler::PriorityScheduler()
{
	// TODO: Return the PID of process with highest prority. 
	return 0;
}

////original write to RAM
//bool Scheduler::WriteNewProcessToRAM(PCB* pcb)
//{
//	//We will change this when we get the MMU
//	for (int i = 0; i < theOS->m_Computer->m_CPU[0].m_PCB->ProgramSize; i++)
//	{
//		//We need to have check and make sure that the new process isn't overwriting an old process. 
//		theOS->m_Computer->m_RAM.write(i, theOS->m_Computer->m_Disk.read(i, pcb->StartIndexDisk), 0);
//
//	}
//	return true;
//}

//takes care of writing multiple processes in RAM by offset
int Scheduler::WriteNewProcessToRAM(PCB* pcb, int offset) 
{
		for (int i = 0; i < pcb->totalSpaceInRAM(); i++) 
		{
			theOS->m_Computer->m_RAM.write(i, theOS->m_Computer->m_Disk.read(i, pcb->StartIndexDisk), offset);
		}
	return 0;
	
}

//will be used in dispatcher in ShortTermScheduler.cpp
//old ClearProcessFromRAM
////This will need to be changed when we are running more processes
//bool Scheduler::ClearOldProcessFromRAM(PCB* pcb)
//{
//	if (!pcb->isExecuting())
//	{
//		for (int i = 0; i < pcb->totalSpaceInRAM(); i++)
//		{
//			theOS->m_Computer->m_RAM.write(i, NULL, pcb->StartIndexRAM);
//		}
//		return true;
//	}
//	return false;
//}

//moved to ShortTermScheduler
//void Scheduler::ClearOldProcessFromRAM(PCB* pcb)
//{
//	for (int i = 0; i < pcb->totalSpaceInRAM(); i++)
//	{
//		theOS->m_Computer->m_RAM.write(i, NULL, pcb->StartIndexRAM);
//	}
//}

bool Scheduler::ExecuteJobQueue() 
{
	int offset;
	do 
	{
		int tempProcessID = m_JobQueue.pop();
		PCB* tempPCB = theOS->m_PCB_Map.at(tempProcessID);
		//int RAMSize = theOS->m_Computer->m_RAM.GetSize; couldn't figure out how to get RAM size
		int RAMSize = 1024;
		if (tempPCB->totalSpaceInRAM() <= (RAMSize - offset)) //checks if there is enough space to put the whole process and io buffers in ram before writing
		{
			WriteNewProcessToRAM(tempPCB, offset);
			offset = offset + tempPCB->totalSpaceInRAM();
			tempPCB->state = Ready;
			theOS->m_ReadyQueue.push(tempPCB);//add PCB of jobs that are in RAM into the ready queue
		}
		if ((tempPCB->totalSpaceInRAM() + offset) > RAMSize) //once the first group of Jobs goes through RAM and is cleared via dispatcher, we will have to start adding RAM from the first index of RAM
		{
			offset = 0;//restarts the offset as 0 so the jobs can start to be written to the beginning index of RAM
		}
	} while (!m_JobQueue.empty());
	return true;
}

