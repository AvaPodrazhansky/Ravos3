#pragma once

// TODO: Design scheduler.  First we'll do the simplest long-term Dispatcher.  Afterwards we'll improve

enum ScheduleType {FIFO = 1, SJF = 2, PRIORITY = 3};

class Scheduler
{
   public:
	   ScheduleType SchedType;

	   std::queue <int> m_JobQueue; //will be filled by FIFo and other scheduling algorithms 
	  
	   Scheduler(OS *_theOS, ScheduleType type = FIFO) { theOS = _theOS; SchedType = type; }
	   
   //PCB* GetPCBBlockAsArray();

	// Dispatches programs that are to be run on the various CPUs. 
	//bool Dispatch(PCB *pcb);

	//we will eventually need to have a method to find an empty location in ram
	//bool WriteFromDiskToRAM(int startIndex, int endIndex);



	
	bool FIFOScheduler();

	int SJFScheduler();

	int PriorityScheduler();

//	bool CheckRAMSpace();

	//bool WriteNewProcessToRAM(PCB* pcb);
	bool WriteNewProcessToRAM(PCB* pcb, int offset); 

	//moved to ShortTermScheduler
	//void ClearOldProcessFromRAM(PCB* pcb);

	//goes through m_JobQueue and writes processes into RAM until m_JobQueue is empty
	bool FillReadyQueue();

	/*bool fillReadyQueue();*/

private:
	OS *theOS = NULL;
};