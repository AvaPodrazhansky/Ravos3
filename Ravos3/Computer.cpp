#include "pch.h"

Computer::Computer(int _NumOfCPU) : m_RAM(MAX_RAM), m_Disk(MAX_DISK)
{
	m_CPU = new CPU[_NumOfCPU];
	NumOfCPU = _NumOfCPU;
	//Initialize all CPU's to point to the same RAM and Disk
	for (int i = 0; i < NumOfCPU; ++i)
	{
		m_CPUMetrics = new CPUMetrics();
		m_CPU[i].m_Disk = &m_Disk;
		m_CPU[i].m_Memory = &m_RAM;
		m_CPUMetrics_Map.insert(std::pair<int, CPUMetrics*>(i, m_CPUMetrics));
		m_CPU[i].CPU_ID = i;
	}
	//m_CPU[0].all_jobs_done = false;
}
