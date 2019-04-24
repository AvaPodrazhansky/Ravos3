#include "pch.h"


ScheduleType PCB_Compare::sType = FIFO;


bool PCB_Compare::operator () (const PCB* x, const PCB* y) const
{
	switch (sType)
	{
	case PRIORITY:
	{
		if (x->getPriority() > y->getPriority()) return true;
		if (x->getPriority() < y->getPriority()) return false;
		break;
	}
	case SJF:
	{
		if (x->getProgramSize() > y->getProgramSize()) return true;
		if (x->getProgramSize() < y->getProgramSize()) return false;
		break;
	}
	default:
		break;
	}
	if (x->getProcessID() > y->getProcessID()) 
		return true;
	
	return false;
}

bool Scheduler::FillJobQueue()
{
	if (m_JobQueue.empty())
	{
		PCB_Compare temp;
		temp.sType = SchedType;

		for (unsigned int i = 1; i <= theOS->m_PCB_Map.size(); i++)
		{
			PCB *b = theOS->m_PCB_Map.at(i);
			m_JobQueue.push(b);
		}

	}

	return true;
}



//takes care of writing multiple processes in RAM by offset
bool Scheduler::WriteWholeProcessToRAM(PCB* pcb, int offset)
{
	pcb->setStartIndexRAM(offset);
	for (int i = 0; i < pcb->totalSpaceInRAM(); i++)
	{
		theOS->m_Computer->m_RAM.write(i, theOS->m_Computer->m_Disk.read(i, pcb->StartIndexDisk), offset);
	}
	return true;

}


void Scheduler::WriteWholeNextProcessToRAM(int StartIndex)
{
	std::lock_guard<std::mutex> lock(theOS->m_JobQueueLock);
	if (m_JobQueue.empty()) return;

	int tempProcessID = m_JobQueue.top()->getProcessID();
	PCB* tempPCB = theOS->m_PCB_Map.at(tempProcessID);

	if (WriteWholeProcessToRAM(tempPCB, StartIndex))
	{
		theOS->m_ReadyQueue.push(tempPCB);
	}
}


bool Scheduler::FillReadyQueue()
{
	while (!m_JobQueue.empty())
	{
		int tempProcessID = m_JobQueue.top()->getProcessID();
		PCB* tempPCB = theOS->m_PCB_Map.at(tempProcessID);

			for (int i = 0; i < 16; i++)//write the first 4 pages into RAM to start 
			{
				MemoryWord k = theOS->m_Computer->m_Disk.read(i, tempPCB->getStartIndexDisk());
				theOS->m_MMU.WriteOrPageFault(i, k, tempPCB);
			}
			m_JobQueue.pop();
			tempPCB->state = Ready;
			theOS->m_ReadyQueue.push(tempPCB);
	}	

	return true;
}


