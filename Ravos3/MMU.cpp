#include "pch.h"

MMU::MMU(OS *_theOS)
{
	theOS = _theOS;
	for (int i = 0; i < 1024 / 4; i++)
	{
		FrameTracker[i] = -1;
	}
}

void MMU::HandlePageFault(int index, PCB* pcb)
{
	PageStruct *page = &pcb->PageTable[index / 4];
	
	for (int i = 0; i < 256; i++)
	{
		if (FrameTracker[i] <= 0)
		{
			FrameTracker[i] = pcb->getProcessID();
			//write page to RAM
			for (int j = 0; j < 4; j++)
			{
				MemoryWord m = theOS->m_Computer->m_Disk.read(index + j, pcb->getStartIndexDisk());
				theOS->m_Computer->m_RAM.write(i * 4, m, j);
			}

			page->FrameNum = i;
		}
	}
}

MemoryWord MMU::ReadOrPageFault(int index, PCB* pcb)
{
	PageStruct *page = &pcb->PageTable[index / 4];
	
	// If no Page Fault
	if (page->FrameNum >= 0)
		return theOS->m_Computer->m_RAM.read(page->FrameNum * 4, index / 4);
	else
		//handle page fault
		HandlePageFault(index, pcb);
		//read from new frame num
	return theOS->m_Computer->m_RAM.read(page->FrameNum * 4, index / 4);
}


void MMU::WriteOrPageFault(int index, PCB* pcb)
{

}