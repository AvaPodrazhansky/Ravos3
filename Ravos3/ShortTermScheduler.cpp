#include "pch.h"
//#include "ShortTermScheduler.h"



//original dispatcher
//bool ShortTermScheduler::Dispatch(PCB *pcb)
//{
//	//copy process from ReadyQueue from disk to RAM (if that funciton returns true, then switch CPU's pointers. Then return true)
//	//if (WriteNewProcessToRAM(pcb));
//	
//	theOS->m_Computer->m_CPU[0].m_PCB = pcb;
//	theOS->m_Computer->m_CPU[0].m_PC = 0; // m_Computer->m_CPU[0].m_PCB->StartIndexRAM;
//	return theOS->m_Computer->m_CPU[0].Execute();//this will be moved to the short term scheduler
//}

void ShortTermScheduler::ClearOldProcessFromRAM(PCB * pcb)
{
	for (int i = 0; i < pcb->totalSpaceInRAM(); i++)
	{
		//if at start of input buffer, write to disk
		if (i >= (pcb->getStartIndexRAM() + pcb->getProgramSize()))
		{
			theOS->m_Computer->m_Disk.write(i, theOS->m_Computer->m_RAM.read(i, pcb->getStartIndexRAM()), pcb->getStartIndexDisk());
		}
		//deletes from RAM
		theOS->m_Computer->m_RAM.write(i, NULL, pcb->getStartIndexRAM());
	}
}

//will need to be changed when using multiple CPU's, could possibly pass the CPU number as param in future
bool ShortTermScheduler::Dispatch()
{
	//copy process from ReadyQueue from disk to RAM (if that funciton returns true, then switch CPU's pointers. Then return true) **Not sure what you meant here**
	//if (WriteNewProcessToRAM(pcb));
	do
	{
		if(theOS->m_Computer->m_CPU[0].m_C_State == IDLE)	
		{
			theOS->m_Computer->m_CPU[0].m_PCB = theOS->m_ReadyQueue.front(); //front returns the next value
			theOS->m_Computer->m_CPU[0].m_CPUMetrics = theOS->m_Computer->m_CPUMetrics_Map.at(0);
			theOS->m_ReadyQueue.pop(); //pop removes the next value (there is no return type)
			//theOS->m_Computer->m_CPU[0].m_PC = 0; 
			theOS->m_Computer->m_CPU[0].m_PCB->getStartIndexRAM();
			//Metrics::updateWaitTime(theOS->m_Computer->m_CPU[0].m_PCB);
			theOS->m_Computer->m_CPU[0].m_PCB->setWaitTime();//should be Metrics::
			
			theOS->m_Computer->m_CPU[0].Execute();//this will be moved to the short term scheduler
			if (theOS->m_Computer->m_CPU[0].m_PCB->state == Terminated) //if process successfully executed
			{
				ShortTermScheduler::ClearOldProcessFromRAM(theOS->m_Computer->m_CPU[0].m_PCB);//removes from RAM
			
				//**Add TerminatedProcesses to Newly Terminated Processes
			}
		}
	} while (!theOS->m_ReadyQueue.empty());
	return true;//returns true when ready queue is empty 
}



int ShortTermScheduler::getAverageWaitTime()
{
	int AverageWaitTime = TOTAL_WAIT_TIME / TOTAL_JOBS_EXECUTED;
	return AverageWaitTime;
}

int ShortTermScheduler::getAverageCompletionTime()
{
	int AverageCompletionTime = TOTAL_COMPLETION_TIME / TOTAL_JOBS_EXECUTED;
	return AverageCompletionTime;
}
//
//ShortTermScheduler::~ShortTermScheduler()
//{
//}
