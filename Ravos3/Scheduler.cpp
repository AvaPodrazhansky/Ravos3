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
bool Scheduler::WriteNewProcessToRAM(PCB* pcb, int offset)
{
	pcb->setStartIndexRAM(offset);
	for (int i = 0; i < pcb->totalSpaceInRAM(); i++)
	{
		//theOS->m_Computer->m_RAM.write(i, theOS->m_Computer->m_Disk.read(i, pcb->StartIndexDisk, -2, -2), offset, -2, -2);
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



bool Scheduler::FillReadyQueue()
{
	using namespace std::literals::chrono_literals; //we should probably move this. Ask Rebekah how this will effect metrics if we move to header file
	
	int offset = 0;
	int RAMSize = theOS->m_Computer->m_RAM.GetSize();

	while (!m_JobQueue.empty())
	{
		////wait while there are no jobs in the job queue
		//while (m_JobQueue.empty())
		//{
		//	std::this_thread::sleep_for(1s);
		//}
		int tempProcessID = m_JobQueue.top()->getProcessID();
		PCB* tempPCB = theOS->m_PCB_Map.at(tempProcessID);

		if (theOS->m_MMU.ManageMemory == true)
		{
			for (int i = 0; i < 16; i++)//write the first 4 pages into RAM to start 
			{
				MemoryWord k = theOS->m_Computer->m_Disk.read(i, tempPCB->getStartIndexDisk());
				theOS->m_MMU.WriteOrPageFault(i, k, tempPCB);
			}
			m_JobQueue.pop();
			tempPCB->state = Ready;
			theOS->m_ReadyQueue.push(tempPCB);
		}
		else 
		{
			if (tempPCB->totalSpaceInRAM() + offset <= RAMSize) //checks if there is enough space to put the whole process and io buffers in ram before writing
			{
				if (WriteNewProcessToRAM(tempPCB, offset))
				{
					m_JobQueue.pop();
					offset = offset + tempPCB->totalSpaceInRAM();
					tempPCB->state = Ready;
					theOS->m_ReadyQueue.push(tempPCB);//add PCB of jobs that are in RAM into the ready queue
				//std::cout << "Job pushed to ready queue\n";
				}
			}
			else if ((tempPCB->totalSpaceInRAM() + offset) > RAMSize) //once the first group of Jobs goes through RAM and is cleared via dispatcher, we will have to start adding RAM from the first index of RAM
			{
				return true;//if RAM is full 
				//offset = 0;//restarts the offset as 0 so the jobs can start to be written to the beginning index of RAM
			}
		}
		////This will change once paging is implemented
		//if (tempPCB->totalSpaceInRAM() + offset > RAMSize)
		//{
		//	//if(!theOS->m_Finished_PCBs.empty())
		//	//	std::this_thread::sleep_for(2s);
		//	//else
		//	//{
		//	//	theOS->m_ShortTerm.ClearOldProcessFromRAM(theOS->m_Finished_PCBs.front());
		//	//}
		//	return true;
		//}
		//////wait while thwere isn't enough space in RAM for job
		////while (tempPCB->totalSpaceInRAM() + offset > RAMSize && !theOS->allJobsExecuted())
		////{
		////	std::this_thread::sleep_for(1s);
		////}

		//if (tempPCB->totalSpaceInRAM() + offset <= RAMSize) //checks if there is enough space to put the whole process and io buffers in ram before writing
		//{
		//	if (WriteNewProcessToRAM(tempPCB, offset))
		//		m_JobQueue.pop();
		//	offset = offset + tempPCB->totalSpaceInRAM();
		//	tempPCB->state = Ready;
		//	theOS->m_ReadyQueue.push(tempPCB);//add PCB of jobs that are in RAM into the ready queue
		//	//std::cout << "Job pushed to ready queue\n";
		//}


		//if ((tempPCB->totalSpaceInRAM() + offset) > RAMSize) //once the first group of Jobs goes through RAM and is cleared via dispatcher, we will have to start adding RAM from the first index of RAM
		//{
		//	return true;//if RAM is full 
		//	//offset = 0;//restarts the offset as 0 so the jobs can start to be written to the beginning index of RAM
		//}

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