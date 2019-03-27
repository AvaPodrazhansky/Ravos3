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
		if (pcb->state == State::Ready)
		{
			theOS->m_ShortTerm.Dispatch(pcb);
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