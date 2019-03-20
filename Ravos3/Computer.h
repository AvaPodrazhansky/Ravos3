#pragma once

// Actual hardware that everything runs on

#define NUM_CPU      1
#define MAX_RAM      1024
#define MAX_DISK	 2048

class Computer
{
public:
	Computer();

	// Returns the number of CPU's in the computer
	int GetNumCPUs() { return NUM_CPU; }



	Memory m_RAM;
	Memory m_Disk;

	CPU    m_CPU[NUM_CPU];
};