#include "pch.h"

MMU::MMU(OS *_theOS, bool _ManageMemory)
{
	theOS = _theOS;
	ManageMemory = _ManageMemory;
	for (int i = 0; i < 1024 / 4; i++)
	{
		FrameTracker[i] = -1;
	}

	for (int i = 0; i < 31; i++)
	{
		OccuranceTracker[i] = 0;
	}
}

void MMU::printValidPages(PCB *pcb)
{
	printf("Job :%4d Instruction    Page    Frame\n", pcb->getProcessID());

	for (int i = 0; i < 18; i++)
	{
		if (&pcb->PageTable[i].FrameNum >= 0)
		{
			for (int j = 0; j < 4; j++)
			{
				int index = i * 4;
				int offset = pcb->PageTable[i].FrameNum * 4;
				printf("%10d %10X %5d %5d\n", pcb->getProcessID(), theOS->m_Computer->m_RAM.read(j, offset), i, &pcb->PageTable[i].FrameNum);
			}
		}
	}
}

void MMU::printPage(int FrameNumber)
{
	PCB *pcb = theOS->m_PCB_Map.at(FrameTracker[FrameNumber]);
	int pageNum = -1;
	for (int i = 0; i < 18; i++)
	{
		if (pcb->PageTable[i].FrameNum == FrameNumber)
			pageNum = i;
	}

	for (int i = 0; i < 4; i++)
	{
		printf("%4d %10X %5d %5d %5d\n", FrameTracker[FrameNumber], theOS->m_Computer->m_RAM.read(i, FrameNumber * 4), pageNum, FrameNumber, pageNum*4+i);
	}
}

void MMU::printValidFrames()
{
	printf("%4s %10s %5s %5s\n", "Job", "Instruction", "Page", "Frame", "Index");

	for (int i = 0; i < 1024 / 4; i++)
	{
		if (FrameTracker[i] > 0)
		{
			printPage(i);
		}
	}
}

void MMU::RemovePagesFromRAM(PCB* pcb)
{
	//for (int i = 0; i < 1024 / 4; i++)
	//{
	//	if (FrameTracker[i] == pcb->getProcessID())
	//	{

	//		std::lock_guard<std::mutex> lock(m_Lock);
	//		FrameTracker[i] = -1;
	//	}
	//}
	for (int i = 0; i < 18; i++)
	{
		if (pcb->PageTable[i].FrameNum >= 0)
		{
			std::lock_guard<std::mutex> lock(m_Lock);
			for (int j = 0; j < 4; j++)
			{
				
				//write to disk
				MemoryWord w = theOS->m_Computer->m_RAM.read(j, pcb->PageTable[i].FrameNum * 4);
				theOS->m_Computer->m_Disk.write(i * 4 + j, w, pcb->getStartIndexDisk());
				//delete from ram
				theOS->m_Computer->m_RAM.write(j, MemoryWord(0), pcb->PageTable[i].FrameNum * 4);
			}
			//Allow other processes to use frame
			//std::lock_guard<std::mutex> lock(m_Lock);
			OccuranceTracker[pcb->getProcessID()]--;
			FrameTracker[pcb->PageTable[i].FrameNum] = -1;
		}
	}

}

void MMU::DeleteSinglePage(PCB* pcb, int PageNumber)
{
	int FrameNumber = pcb->PageTable[PageNumber].FrameNum;

	//Allow other processes to use frame
	std::lock_guard<std::mutex> lock(m_Lock);
	OccuranceTracker[pcb->getProcessID()]--;
	FrameTracker[FrameNumber] = -1;
	pcb->PageTable[PageNumber].FrameNum = -1;


	for (int j = 0; j < 4; j++)
	{
		//write to disk
		MemoryWord w = theOS->m_Computer->m_RAM.read(j, FrameNumber * 4);
		theOS->m_Computer->m_Disk.write(PageNumber * 4 + j, w, pcb->getStartIndexDisk());
		//delete from ram
		theOS->m_Computer->m_RAM.write(j, MemoryWord(0), FrameNumber * 4);
	}


}

int MMU::DeleteLeastRecentlyUsedPage()
{
	int max = 0;
	int highestPCB = 0;
	for (int i = 0; i < 31; i++)
	{
		if (OccuranceTracker[i] > max)
		{
			max = OccuranceTracker[i];
			highestPCB = i;
		}
	}

	PCB *pcb = theOS->m_PCB_Map.at(highestPCB);

	int MaxPageSince = 0;
	auto stop = std::chrono::high_resolution_clock::now();//end time of process
	auto maxSince = std::chrono::duration_cast<std::chrono::milliseconds>(stop - pcb->PageTable[0].timeSinceUsed);

	for (int i = 1; i < 18; i++)
	{
		auto timeSince = std::chrono::duration_cast<std::chrono::milliseconds>(stop - pcb->PageTable[i].timeSinceUsed);
		if (maxSince > timeSince) 
		{
			maxSince = timeSince;
			MaxPageSince = i;
		}
	}
	
	int Frame = pcb->PageTable[MaxPageSince].FrameNum;
	
	DeleteSinglePage(pcb, MaxPageSince);

	return Frame;

}

void MMU::HandlePageFault(int index, PCB* pcb)
{
	PageStruct *page = &pcb->PageTable[index / 4];
	
	for (int i = 0; i < 256; i++)
	{
		if (FrameTracker[i] <= 0)
		{
			std::lock_guard<std::mutex> lock(m_Lock);
			std::cout << "Frame: " << i << std::endl;
			FrameTracker[i] = pcb->getProcessID();
			//write page to RAM
			for (int j = 0; j < 4; j++)
			{
				MemoryWord m = theOS->m_Computer->m_Disk.read(index + j, pcb->getStartIndexDisk());
				theOS->m_Computer->m_RAM.write(i * 4, m, j);
			}
			page->FrameNum = i;
			OccuranceTracker[pcb->getProcessID()]++;
			printPage(i);
			return;
		}
	}

	//Handle Full RAM
	int newFrame = DeleteLeastRecentlyUsedPage();
	for (int j = 0; j < 4; j++)
	{
		MemoryWord m = theOS->m_Computer->m_Disk.read(index + j, pcb->getStartIndexDisk());
		theOS->m_Computer->m_RAM.write(newFrame * 4, m, j);
	}
	return;

}

MemoryWord MMU::ReadOrPageFault(int index, PCB* pcb)
{
	if(!ManageMemory)
		return theOS->m_Computer->m_RAM.read(index, pcb->getStartIndexRAM);

	PageStruct *page = &pcb->PageTable[index / 4];
	
	// If no Page Fault
	if (page->FrameNum >= 0)
	{
		page->timeSinceUsed = std::chrono::high_resolution_clock::now();
		return theOS->m_Computer->m_RAM.read(page->FrameNum * 4, index / 4);
	}

	else
	{	//handle page fault
		std::cout << "Page Fault\n";
		HandlePageFault(index, pcb);
		//read from new frame num
		page->timeSinceUsed = std::chrono::high_resolution_clock::now();
		return theOS->m_Computer->m_RAM.read(page->FrameNum * 4, index / 4);
	}
}


void MMU::WriteOrPageFault(int index, MemoryWord w, PCB* pcb)
{
	if (!ManageMemory)
		return theOS->m_Computer->m_RAM.read(index, pcb->getStartIndexRAM);

	PageStruct *page = &pcb->PageTable[index / 4];

	// If no Page Fault
	if (page->FrameNum >= 0)
	{
		//theOS->m_Computer->m_RAM.write(page->FrameNum * 4, w, index / 4);
		page->timeSinceUsed = std::chrono::high_resolution_clock::now();
		theOS->m_Computer->m_RAM.write(index % 4, w, page->FrameNum * 4);
	}
	else
	{	//handle page fault
		HandlePageFault(index, pcb);
		//read from new frame num
		page->timeSinceUsed = std::chrono::high_resolution_clock::now();
		theOS->m_Computer->m_RAM.write(index % 4, w, page->FrameNum * 4);
	}
}