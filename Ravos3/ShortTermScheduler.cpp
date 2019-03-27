#include "pch.h"
//#include "ShortTermScheduler.h"


ShortTermScheduler::ShortTermScheduler(OS *_theOS)
{
	theOS = _theOS;
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
