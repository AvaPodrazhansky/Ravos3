#pragma once

class MMU
{
	friend class Scheduler;
	friend class ShortTermScheduler;
private:
	OS *theOS = NULL;

	// PageStruct m_PageTable;
	std::mutex m_Lock;
	int FrameTracker[1024 / PAGE_SIZE];

	bool ManageMemory;

	int OccuranceTracker[31];
	//Memory m_RAM;
	//Memory m_Disk;

public:
	MMU(OS *_theOS, bool _ManageMemory = false);

	MemoryWord ReadOrPageFault(int index, PCB* pcb);

	void HandlePageFault(int index, PCB* pcb);

	void WriteOrPageFault(int index, MemoryWord w, PCB* pcb);

	void RemovePagesFromRAM(PCB* pcb);

	void printValidPages(PCB *pcb);
	
	void printValidFrames();

	void printPage(int FrameNumber);
	
	void DeleteSinglePage(PCB* pcb, int PageNumber);

	int DeleteLeastRecentlyUsedPage();

	void AssignPageToFrame(int FrameNumber, int index, PCB* pcb);
};
