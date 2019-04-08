#pragma once
#include "pch.h"

class OS 
{
	//friend class ShortTermScheduler;
//	friend FIFO_Compare;
	friend Scheduler;
public:
	Computer *m_Computer;
	ShortTermScheduler m_ShortTerm;
	Scheduler m_Scheduler;

	OS(Computer *theComputer);

	std::map<int, PCB*> m_PCB_Map; //map is the C++ version of dictionary

	std::queue <PCB*> m_ReadyQueue; //Queue of PCB's

	//std::queue <int> m_CPU_ReadyQueue; //Queue of CPU ID's

	std::queue <PCB*> m_Finished_PCBs; //Queue of PCB's
	
	std::map<int, PCB*> m_Terminated_PCBs;

	int busyCPUs = 0;

	bool allJobsExecuted();

	void Schedule_and_Run(ScheduleType default_type = FIFO);

	// Initilizes the computer.  A Several things are happening:
	//    (1) Uses Loader routines to read the boot file (containing the jobs) onto the disk
	//    (2) Creates a PCB for each job
	// Important: We are creating and filling the disk because our storage is does not survive reboots.
	bool Boot(std::string filename);

	// Loads the boot file onto the disk of the computer.
	bool Load(std::string filename);

	int HexNumToInt(std::string hexstr);
	int assignPCB(PCB *tpcb, std::string info);
	int assignDataBuffToPCB(PCB* tpcb, std::string info, int startIndex);

	void StartShortTermScheduler();

	// Test loader - until actual loader is done.  Does not work yet
	// It is located in the 
	//void TestLoad1();


};
