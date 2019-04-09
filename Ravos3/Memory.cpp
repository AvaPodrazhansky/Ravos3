#include "pch.h"


Memory::Memory(int SIZE, int _VirtualMemory)
{
	Size = SIZE;
	memory = new MemoryWord[Size];
	Clear();

	VirtualMemory = _VirtualMemory;

	if (VirtualMemory >0)
	{
		//PageMap = (PageStruct*) calloc(VirtaulMemory / PAGE_SIZE, sizeof(PageStruct));
		PageMap = new PageStruct[VirtualMemory / PAGE_SIZE];
		FrameMap = new int[Size];
		memset(FrameMap, 0, sizeof(int)*Size);
	}
}

void Memory::Clear()
{
	for (int i = 0; i < Size; ++i)
		memory[i].Clear();
}

int Memory::MapToFrame(int PageNum, int ProcessID, int PC)
{
	if (!PageMap) return PageNum;
	if (PageNum < 0 || PageNum >= VirtualMemory / PAGE_SIZE)
		throw RavosInvalidMemoryAddressException(ProcessID, PageNum, PC);

	PageStruct *Page = &PageMap[PageNum];

	if (ProcessID >= 0)
		if (Page->ProcessID != ProcessID) throw RavosMemoryProtectionException(ProcessID, PageNum, PC);

	if (Page->FrameNum >= 0) return Page->FrameNum;

	// We have a page fault
	throw RavosPagingFaultException(ProcessID, PageNum, PC);
	return 0;
}

void Memory::write(int index, MemoryWord t, int Offset, int PID, int PC)
{
	assert(PID >= 0 || PID == -2);
	assert(PC >= 0 || PC == -2);

	index += Offset;

	if (!PageMap) 
	{
		if (index < 0 || index >= Size)
			throw RavosInvalidMemoryAddressException(PID, index/4, PC);
		std::lock_guard<std::mutex> lock(LockMutex);
		memory[index] = t;
	}
	else
	{
		int PageOffset = index % PAGE_SIZE;
		int Loc = MapToFrame(index / PAGE_SIZE) + PageOffset;

		std::lock_guard<std::mutex> lock(LockMutex);
		memory[Loc] = t;
	}
}

MemoryWord Memory::read(int index, int Offset, int PID, int PC)
{
	assert(PID>=0 || PID==-2);
	assert(PC >= 0 || PC == -2);

	index += Offset;

	if (!PageMap)
	{
		if (index < 0 || index >= Size)
			throw RavosInvalidMemoryAddressException(PID, index / 4, PC);
		std::lock_guard<std::mutex> lock(LockMutex);

		return memory[index];
	}
	else
	{
		int PageOffset = index % PAGE_SIZE;
		int Loc = MapToFrame(index / PAGE_SIZE) + PageOffset;

		std::lock_guard<std::mutex> lock(LockMutex);
		return memory[Loc];
	}
}

void Memory::clearEverything()
{
	for (int i = 0; i < Size; i++)
	{
		memory[i] = NULL;
	}
}




