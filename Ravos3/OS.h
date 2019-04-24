#pragma once
#include "pch.h"

class OS 
{
	friend Scheduler;
public:
	Computer *m_Computer;
	ShortTermScheduler m_ShortTerm;
	Scheduler m_Scheduler;
	MMU m_MMU;
	OS(Computer *theComputer, bool UseMMU=false);

	std::map<int, PCB*> m_PCB_Map; //map is the C++ version of dictionary

	std::queue <PCB*> m_ReadyQueue; //Queue of PCB's

	std::map<int, PCB*> m_Terminated_PCBs;

	bool allJobsExecuted();

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


	std::mutex m_JobQueueLock;

};
