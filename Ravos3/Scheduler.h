#pragma once

enum ScheduleType {FIFO = 1, SJF = 2, PRIORITY = 3};


class PCB_Compare : std::less<PCB*>
{
public:
	static enum ScheduleType sType;

	bool operator () (const PCB* x, const PCB* y) const;

};

class Scheduler
{
	private:
		OS *theOS = NULL;

	public:
	   ScheduleType SchedType = FIFO;
	   std::priority_queue<PCB*, std::vector<PCB*>, PCB_Compare> m_JobQueue;
	   
	   Scheduler(OS *_theOS, ScheduleType type = FIFO) 
	   { 
		   theOS = _theOS; 
		   SchedType = type; 
	   }
	   
	// Dispatches programs that are to be run on the various CPUs. 
	//bool Dispatch(PCB *pcb);
	  

	bool FillJobQueue();

//	bool CheckRAMSpace();

	//bool WriteWholeProcessToRAM(PCB* pcb);
	bool WriteWholeProcessToRAM(PCB* pcb, int offset); 

	//moved to ShortTermScheduler
	//void ClearOldProcessFromRAM(PCB* pcb);

	//goes through m_JobQueue and writes processes into RAM until m_JobQueue is empty
	bool FillReadyQueue();

	/*bool fillReadyQueue();*/


};

