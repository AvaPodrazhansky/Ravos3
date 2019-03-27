#pragma once


// TODO: Design scheduler.  First we'll do the simplest long-term Dispatcher.  Afterwards we'll improve

enum ScheduleType {FIFO = 1, SJF = 2, PRIORITY = 3};

class Scheduler
{
   public:

	   Scheduler(OS *_theOS, ScheduleType  = FIFO) { theOS = _theOS; }
	   
	   
	   std::queue <int> m_ReadyQueue;

   //PCB* GetPCBBlockAsArray();

	// Dispatches programs that are to be run on the various CPUs. 
	bool Dispatch(PCB *pcb);

	//we will eventually need to have a method to find an empty location in ram
	//bool WriteFromDiskToRAM(int startIndex, int endIndex);

	//This is for when a process is finished executing
	bool ClearOldProcessFromRAM(PCB* pcb);

	//Wrtie new process to RAM
	bool WriteNewProcessToRAM(PCB* pcb);

	//Returns the PID of next program to be executed.
	int FIFOScheduler();

	int SJFScheduler();

	int PriorityScheduler();

private:
	OS *theOS = NULL;
};