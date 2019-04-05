#include "pch.h"

Computer::Computer() : m_RAM(MAX_RAM), m_Disk(MAX_DISK)
{
	//Initialize all CPU's to point to the same RAM and Disk
	for (int i = 0; i < GetNumCPUs(); ++i)
	{
		m_CPUMetrics = new CPUMetrics();
		m_CPU[i].m_Disk = &m_Disk;
		m_CPU[i].m_Memory = &m_RAM;
		m_CPUMetrics_Map.insert(std::pair<int, CPUMetrics*>(i, m_CPUMetrics));
	}
}
