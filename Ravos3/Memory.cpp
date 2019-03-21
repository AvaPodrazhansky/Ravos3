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

void Memory::write(int index, MemoryWord t)
{
	assert(index >= 0 && index < Size);
	memory[index] = t;
}

MemoryWord Memory::read(int index)
{
	assert(index >= 0 && index < Size);
	return memory[index];
}



