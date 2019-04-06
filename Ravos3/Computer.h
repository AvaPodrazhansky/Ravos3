#pragma once
// Actual hardware that everything runs on

#define NUM_CPU      1
#define MAX_RAM      1024
#define MAX_DISK	 2048

static bool WRITE_KEY = false;
static bool READ_KEY = true;

class Computer
{
public:
	Computer();

	// Returns the number of CPU's in the computer
	int GetNumCPUs() { return NUM_CPU; }



	Memory m_RAM;
	Memory m_Disk;

	CPU    m_CPU[NUM_CPU];
	Memory *Cache[28];//has not been implemented
	CPUMetrics* m_CPUMetrics;

	std::map<int, CPUMetrics*> m_CPUMetrics_Map; //map to link CPU with it's metrics
};