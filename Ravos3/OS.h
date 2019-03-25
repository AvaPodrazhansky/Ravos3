#pragma once
#include "pch.h"
class OS
{
	//friend class Scheduler;
public:
	Computer *m_Computer;
	OS(Computer *theComputer) { m_Computer = theComputer; };


	std::map<int, PCB> m_PCB_Map; //map is the C++ version of dictionary

	void tempDispatcher()
	{
		for (auto elem : m_PCB_Map)
		{
			std::cout << "PID: " << elem.first << " \n";// << elem.second. << "\n";
		}
		//m_Computer->m_CPU[0].m_PCB_Map = m_PCB_Map.at(1);
		//m_Computer->m_CPU[0].m_PC = m_PCB_Map.at(1)->StartIndexRAM;
		//m_Computer->m_CPU[0].Execute();
	}


	Scheduler m_Scheduler;

	// Initilizes the computer.  A Several things are happening:
	//    (1) Uses Loader routines to read the boot file (containing the jobs) onto the disk
	//    (2) Creates a PCB for each job
	// Important: We are creating and filling the disk because our storage is does not survive reboots.
	bool Boot(std::string filename);

	//int HexNumToInt(std::string hexstr);
	//int assignPCB(PCB *tpcb, std::string info);
	//int assignDataBuffToPCB(PCB* tpcb, std::string info, int startIndex);

	// Loads the boot file onto the disk of the computer.
	// It is located in the file OS_Loader (so a different people can work on it)	
	//bool Load(std::string filename, Computer *m_Comp);
	bool Load(std::string filename);

	int HexNumToInt(std::string hexstr);
	int assignPCB(PCB *tpcb, std::string info);
	int assignDataBuffToPCB(PCB* tpcb, std::string info, int startIndex);

	// Test loader - until actual loader is done.  Does not work yet
	// It is located in the 
	void TestLoad1();


};
