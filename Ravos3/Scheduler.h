#pragma once

// TODO: Design scheduler.  First we'll do the simplest long-term Dispatcher.  Afterwards we'll improve

enum ScheduleType {FIFO = 1, SJF = 2, PRIORITY = 3};

class Scheduler
{
   public:
	   ScheduleType SchedType;
	  
	   Scheduler(OS *_theOS, ScheduleType type = FIFO) { theOS = _theOS; SchedType = type; }
	   
	   std::queue <int> RAM_queue;
	   RAM_queue.push(0);
   //PCB* GetPCBBlockAsArray();

	// Dispatches programs that are to be run on the various CPUs. 
	//bool Dispatch(PCB *pcb);

	//we will eventually need to have a method to find an empty location in ram
	//bool WriteFromDiskToRAM(int startIndex, int endIndex);



	//Returns the PID of next program to be executed.
	int FIFOScheduler();

	int SJFScheduler();

	int PriorityScheduler();

	bool WriteNewProcessToRAM(PCB* pcb);

	bool ClearOldProcessFromRAM(PCB* pcb);

	bool fillReadyQueue();

private:
	OS *theOS = NULL;
};