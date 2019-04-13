#include "pch.h"

MMU::MMU(OS *_theOS, bool _ManageMemory)
{
	theOS = _theOS;
	ManageMemory = _ManageMemory;
	for (int i = 0; i < 1024 / 4; i++)
	{
		FrameTracker[i] = -1;
	}
}

void MMU::RemovePagesFromRAM(PCB* pcb)
{
	//for (int i = 0; i < 1024 / 4; i++)
	//{
	//	if (FrameTracker[i] == pcb->getProcessID())
	//	{

	//		std::lock_guard<std::mutex> lock(LockFrameTracker);
	//		FrameTracker[i] = -1;
	//	}
	//}
	for (int i = 0; i < 18; i++)
	{
		if (pcb->PageTable[i].FrameNum >= 0)
		{
			for (int j = 0; j < 4; j++)
			{
				//write to disk
				MemoryWord w = theOS->m_Computer->m_RAM.read(j, pcb->PageTable[i].FrameNum * 4);
				theOS->m_Computer->m_Disk.write(i * 4 + j, w, pcb->getStartIndexDisk());
				//delete from ram
				theOS->m_Computer->m_RAM.write(j, MemoryWord(0), pcb->PageTable[i].FrameNum * 4);
			}
			//Allow other processes to use frame
			std::lock_guard<std::mutex> lock(LockFrameTracker);
			FrameTracker[pcb->PageTable[i].FrameNum] = -1;
		}
	}

}


void MMU::HandlePageFault(int index, PCB* pcb)
{
	PageStruct *page = &pcb->PageTable[index / 4];
	
	for (int i = 0; i < 256; i++)
	{
		if (FrameTracker[i] <= 0)
		{
			std::lock_guard<std::mutex> lock(LockFrameTracker);
			FrameTracker[i] = pcb->getProcessID();
			//write page to RAM
			for (int j = 0; j < 4; j++)
			{
				MemoryWord m = theOS->m_Computer->m_Disk.read(index + j, pcb->getStartIndexDisk());
				theOS->m_Computer->m_RAM.write(i * 4, m, j);
			}

			page->FrameNum = i;
			return;
		}
	}

	//Handle Full RAM
}

MemoryWord MMU::ReadOrPageFault(int index, PCB* pcb)
{
	PageStruct *page = &pcb->PageTable[index / 4];
	
	// If no Page Fault
	if (page->FrameNum >= 0)
		return theOS->m_Computer->m_RAM.read(page->FrameNum * 4, index / 4);
	else
	{	//handle page fault
		HandlePageFault(index, pcb);
		//read from new frame num
		return theOS->m_Computer->m_RAM.read(page->FrameNum * 4, index / 4);
	}
}


void MMU::WriteOrPageFault(int index, MemoryWord w, PCB* pcb)
{
	PageStruct *page = &pcb->PageTable[index / 4];

	// If no Page Fault
	if (page->FrameNum >= 0)
		theOS->m_Computer->m_RAM.write(page->FrameNum * 4, w, index / 4);
	else
	{	//handle page fault
		HandlePageFault(index, pcb);
		//read from new frame num
		theOS->m_Computer->m_RAM.write(page->FrameNum * 4, w, index / 4);
	}
}