#pragma once

class MMU
{
private:
	OS *theOS = NULL;

	// PageStruct m_PageTable;

	int FrameTracker[1024 / PAGE_SIZE];

	//Memory m_RAM;
	//Memory m_Disk;

public:
	MMU(OS *_theOS);

	MemoryWord ReadOrPageFault(int index, PCB* pcb);

	void HandlePageFault(int index, PCB* pcb);

	void WriteOrPageFault(int index, PCB* pcb);


};
