#include "pch.h"


bool ShortTermScheduler::Start_the_CPU(int cpu)
{
	new std::thread(&CPU::CPU_Run_thread, theOS->m_Computer->m_CPU[cpu]);
	return true;
}


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

	theOS->m_Terminated_PCBs.insert(std::pair<int, PCB*>(pcb->getProcessID(), pcb));
}


bool ShortTermScheduler::Dispatch(int CPU_Num)
{
		if (theOS->m_Computer->m_CPU[CPU_Num].m_C_State == IDLE)
		{
			// Set CPU's PCB to next PCB
			theOS->m_Computer->m_CPU[CPU_Num].assignPCB(theOS->m_ReadyQueue.front()); //front returns the next value
			theOS->m_ReadyQueue.pop(); //pop removes the next value (there is no return type)

			theOS->m_Computer->m_CPU[CPU_Num].m_CPUMetrics = theOS->m_Computer->m_CPUMetrics_Map.at(CPU_Num);//assigns which CPUMetrics is associated with this CPU
			theOS->m_Computer->m_CPU[CPU_Num].m_CPUMetrics->updateJobsInThisCPU(theOS->m_Computer->m_CPU[CPU_Num].m_PCB->getProcessID());//adds the process ID of this job to the vector holding all the jobs that are assigned to this CPU

			theOS->m_Computer->m_CPU[CPU_Num].m_PCB->setWaitTime(theOS->m_Computer->m_CPU[CPU_Num].m_CPUMetrics->getTotalWaitOnThisCPU());
			theOS->m_Computer->m_CPU[CPU_Num].m_CPUMetrics->setTotalWaitOnThisCPU(theOS->m_Computer->m_CPU[CPU_Num].m_PCB->getProgramSize());
			////https://en.cppreference.com/w/cpp/thread/condition_variable
		}
	return true;
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

