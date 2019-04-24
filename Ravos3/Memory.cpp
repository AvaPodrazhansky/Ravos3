#include "pch.h"


Memory::Memory(int SIZE, int _VirtualMemory)
{
	Size = SIZE;
	memory = new MemoryWord[Size];
	Clear();
}

void Memory::Clear()
{
	for (int i = 0; i < Size; ++i)
		memory[i].Clear();
}


void Memory::write(int index, MemoryWord t, int Offset)
{
	index += Offset;
	std::lock_guard<std::mutex> lock(LockMutex);
	memory[index] = t;
}

MemoryWord Memory::read(int index, int Offset)
{
	index += Offset;
	std::lock_guard<std::mutex> lock(LockMutex);
	return memory[index];
}

void Memory::clearEverything()
{
	for (int i = 0; i < Size; i++)
	{
		memory[i] = NULL;
	}
}




