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
//	if(WriteWholeProcessToRAM(pcb));
//	theOS->m_Computer->m_CPU[0].m_PCB = pcb;
//	theOS->m_Computer->m_CPU[0].m_PC = 0; // m_Computer->m_CPU[0].m_PCB->StartIndexRAM;
//	return theOS->m_Computer->m_CPU[0].Execute();//this will be moved to the short term scheduler
//}

ScheduleType PCB_Compare::sType = FIFO;

//bool PCB_Compare::operator () (const PCB & x, const PCB & y) const
//{
//	switch (sType)
//	{
//		case PRIORITY:
//		{
//			if (x.getPriority() > y.getPriority()) return true;
//			break;
//		}
//		case SJF:
//		{
//			if (x.getProgramSize() > y.getProgramSize()) return true;
//			break;
//		}
//		default:
//			break;
//	}
//	if (x.getProcessID() > y.getProcessID()) return true;
//	return false;
//}

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
			m_JobQueue.push(b);//m_PCB_Map.at(i).second);
		}

		//We need to delete temp
	}

	//new std::thread(&FillReadyQueue, this);
	//return FillReadyQueue();
	return true;
}


//bool Scheduler::FIFOScheduler()
//{
////	std::priority_queue<PCB*> q;
//	if (m_JobQueue.empty())
//	{
//		for (unsigned int i = 1; i <= theOS->m_PCB_Map.size(); ++i) //changed i to unsigned becuase map.size() returns size_type
//		{
//			PCB* pcb = theOS->m_PCB_Map[i];
//			//if (pcb->isExecuting())
//			//{
//			
//			/*m_JobQueue.push(pcb->getProcessID());*/
//			m_JobQueue.push(pcb);
//			
//			//theOS->m_ShortTerm.Dispatch(pcb);
//			//if(Dispatch(pcb)); //If Dispatch(pcb) = signal OS to execute
//			//if all CPUs are busy, wait
//		//}
//		}
//
//	}
//
//	return(FillReadyQueue());
//
//}
//
//bool Scheduler::SJFScheduler()
//{
//	return true;
//}
//
//bool Scheduler::PriorityScheduler()
//{
//
//	return true;
//}


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



/*
// See https://docs.microsoft.com/en-us/cpp/dotnet/lock-class?view=vs-2019#acquire

PCB* Scheduler::short_term_get_next_queue()
{
*pcb p = NULL;

   try
   {   
	   lock l(ready_queue);

	   p = ready_queue.First();
	   if (p!=NULL) ready_queue.pop();
	}
	catch
	{
	}
   

   return p;
}


void Scheduler::short_term_add_to_ready(PCB *b)
{
   {
	   lock l(ready_queue);

	   ready_queue.queue(b);
	}
}

*/