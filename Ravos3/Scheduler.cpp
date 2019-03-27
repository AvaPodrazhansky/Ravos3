#include "pch.h"



//PCB* Scheduler::GetPCBBlockAsArray()
//{
//
//	PCB *pcb = calloc(theOS->m_PCB_Map, sizeof(PCB));
//	return pcb;
//
//}

bool Scheduler :: WriteNewProcessToRAM(PCB* pcb)
{
	//We will change this when we get the MMU
	for (int i = 0; i < theOS->m_Computer->m_RAM.GetSize(); i++)
	{
		//We need to have check and make sure that the new process isn't overwriting an old process. 

	}
	return false;
}




//This will need to be changed when we are running more processes
bool Scheduler::ClearOldProcessFromRAM(PCB* pcb)
{
	if (!pcb->isExecuting())
	{
		for (int i = 0; i <= pcb->totalSpaceInRAM(); i++)
		{
			theOS->m_Computer->m_RAM.write(i, NULL, pcb->StartIndexRAM);
		}
		return true;
	}
	return false;
}

// Scheduler implementation
//We will have this return false if we are out of RAM (or if there is another problem loading into RAM)
//This is part of the short term scheudler (I think)
bool Scheduler::Dispatch(PCB *pcb)
{
	//copy process from ReadyQueue from disk to RAM (if that funciton returns true, then switch CPU's pointers. Then return true)
	theOS->m_Computer->m_CPU[0].m_PCB = pcb;
	theOS->m_Computer->m_CPU[0].m_PC = 0; // m_Computer->m_CPU[0].m_PCB->StartIndexRAM;
	return theOS->m_Computer->m_CPU[0].Execute();//this will be moved to the short term scheduler
}


int Scheduler::FIFOScheduler()
{
//	std::priority_queue<PCB*> q;

	for (int i = 0; i < theOS->m_PCB_Map.size(); ++i)
	{
		PCB* pcb = theOS->m_PCB_Map[i];
		if (pcb->state == State::Ready)
		{
			Dispatch(pcb);
			//if(Dispatch(pcb)); //If Dispatch(pcb) = signal OS to execute
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