#pragma once

class OS
{
public:
	OS(Computer *theComputer);

	Computer *m_Computer;
	std::map<int, PCB*> m_PCB; //map is the C++ version of dictionary

	Scheduler m_Scheduler;

	// Initilizes the computer.  A Several things are happening:
	//    (1) Uses Loader routines to read the boot file (containing the jobs) onto the disk
	//    (2) Creates a PCB for each job
	// Important: We are creating and filling the disk because our storage is does not survive reboots.
	bool Boot(const char *Filename);

	// Loads the boot file onto the disk of the computer.
	// It is located in the file OS_Loader (so a different people can work on it)	
	bool Load(std::string filename);

	// Test loader - until actual loader is done.  Does not work yet
	// It is located in the 
	void TestLoad1();


};
