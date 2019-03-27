#include "pch.h"
//#include "ShortTermScheduler.h"


ShortTermScheduler::ShortTermScheduler(OS *_theOS)
{
	theOS = _theOS;
}


bool ShortTermScheduler::WriteNewProcessToRAM(PCB* pcb)
{
	//We will change this when we get the MMU
	for (int i = 0; i < theOS->m_Computer->m_RAM.GetSize(); i++)
	{
		//We need to have check and make sure that the new process isn't overwriting an old process. 
		theOS->m_Computer->m_RAM.write(i, theOS->m_Computer->m_Disk.read(i, pcb->StartIndexDisk), 0);

	}
	return false;
}


//This will need to be changed when we are running more processes
bool ShortTermScheduler::ClearOldProcessFromRAM(PCB* pcb)
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

bool ShortTermScheduler::Dispatch(PCB *pcb)
{
	//copy process from ReadyQueue from disk to RAM (if that funciton returns true, then switch CPU's pointers. Then return true)
	//if (WriteNewProcessToRAM(pcb));
	
	theOS->m_Computer->m_CPU[0].m_PCB = pcb;
	theOS->m_Computer->m_CPU[0].m_PC = 0; // m_Computer->m_CPU[0].m_PCB->StartIndexRAM;
	return theOS->m_Computer->m_CPU[0].Execute();//this will be moved to the short term scheduler
}

//
//ShortTermScheduler::~ShortTermScheduler()
//{
//}
