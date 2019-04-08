#include "pch.h"


Memory::Memory(int SIZE)
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
	assert(index >= 0 && index < Size);
	memory[index] = t;
}

void Memory::clearEverything()
{
	for (int i = 0; i < 1024; i++)
	{
		memory[i] = NULL;
	}
}

MemoryWord Memory::read(int index, int Offset)
{
	index += Offset;
	assert(index >= 0 && index < Size);
	return memory[index];
}



