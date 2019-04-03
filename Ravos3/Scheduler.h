#pragma once

// TODO: Design scheduler.  First we'll do the simplest long-term Dispatcher.  Afterwards we'll improve

enum ScheduleType {FIFO = 1, SJF = 2, PRIORITY = 3};

//enum struct ScheduleType { FIFO, SJF, PRIORITY };

//enum struct ScheduleType { FIFO{bool operator()(PCB* const& p1, PCB* const& p2)
//								   {
//									   return p1->getProcessID() < p2->getProcessID();
//								   }
//								}, SJF, PRIORITY };

//ScheduleType::FIFO()
//{
//	bool operator()(PCB* const& p1, PCB* const& p2)
//	{
//	  return p1->getProcessID() < p2->getProcessID();
//	}
//};

//struct ComparePCB
//{
//	bool operator()(PCB* const& p1, PCB* const& p2, ScheduleType type)
//	{
//		switch (type)
//		{
//		case FIFO:
//		{
//			return p1->getProcessID() < p2->getProcessID();
//			break;
//		}
//		case SJF:
//		{
//			return p1->getProgramSize() < p2->getProgramSize();
//			break;
//		}
//		case PRIORITY:
//		{
//			return p1->getPriority() < p2->getPriority();
//			break;
//		}
//
//		}
//	}
//};

//struct ComparePCB
//{
//	ScheduleType type = FIFO;
//	
//	ComparePCB(ScheduleType _type = FIFO) { type = _type; }
//
//	bool operator()(PCB* const& p1, PCB* const& p2)
//	{
//		switch (type)
//		{
//			case FIFO:
//			{
//				return p1->getProcessID() < p2->getProcessID();
//				break;
//			}
//			case SJF:
//			{
//				return p1->getProgramSize() < p2->getProgramSize();
//				break;
//			}
//			case PRIORITY:
//			{
//				return p1->getPriority() < p2->getPriority();
//				break;
//			}
//
//		}
//	}
//};
//
//struct FIFO_Compare
//{
//	bool operator()(PCB* const& p1, PCB* const& p2)
//	{
//		return p1->getProcessID() < p2->getProcessID();
//	}
//};
/*
template <class PCB> struct less : binary_function <T, T, bool> {
	bool operator() (const T& x, const T& y) const { return x < y; }
};*/

class PCB_Compare : std::less<PCB*>
{
public:
	static enum ScheduleType sType;
	//static ScheduleType sType;
public:
	bool operator () (const PCB* x, const PCB* y) const;
	//bool operator () (const PCB& x, const PCB& y) const;

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
	   
   //PCB* GetPCBBlockAsArray();

	// Dispatches programs that are to be run on the various CPUs. 
	//bool Dispatch(PCB *pcb);
	
	//bool FIFOScheduler();

	//bool SJFScheduler();

	//bool PriorityScheduler();

	bool FillJobQueue();

//	bool CheckRAMSpace();

	//bool WriteNewProcessToRAM(PCB* pcb);
	bool WriteNewProcessToRAM(PCB* pcb, int offset); 

	//moved to ShortTermScheduler
	//void ClearOldProcessFromRAM(PCB* pcb);

	//goes through m_JobQueue and writes processes into RAM until m_JobQueue is empty
	bool FillReadyQueue();

	/*bool fillReadyQueue();*/


};

