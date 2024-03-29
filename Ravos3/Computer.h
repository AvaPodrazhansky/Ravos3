#pragma once
// Actual hardware that everything runs on

#define MAX_RAM         1024
#define MAX_RAM_VIRTUAL 2048
#define MAX_DISK	    2048

static bool WRITE_KEY = false;
static bool READ_KEY = true;

class Computer
{
public:
	int NumOfCPU = 1;

	Computer(int _NumOfCPU);

	// Returns the number of CPU's in the computer
	int GetNumCPUs() { return NumOfCPU; }

	Memory m_RAM;
	Memory m_Disk;

	CPU  *m_CPU;
	CPUMetrics* m_CPUMetrics;

	std::map<int, CPUMetrics*> m_CPUMetrics_Map; //map to link CPU with it's metrics
};