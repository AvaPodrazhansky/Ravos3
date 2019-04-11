#pragma once
#include "pch.h"

#define PAGE_SIZE   4


struct MemoryWord
{
	MemoryWord() {Clear(); }
	void Clear() { Contents = 0; }

	MemoryWord(int TheContents) { Contents = TheContents; }

	union
	{
		// Will work on Intel, reverse order of struct if to switch Endian
		unsigned int Contents;

		struct
		{
			unsigned int ShortAddress : 16;
			unsigned int Reg2 : 4;
			unsigned int Reg1 : 4;
			unsigned int OpCode : 6;
			unsigned int InstType : 2;

			unsigned int GetLongAddress() { return Reg1 * (65536 * 16) + Reg2 * 65536 + ShortAddress; }

			//unsigned int GetAddress() { return InstType < 2 ? ShortAddress : GetLongAddress(); }
			unsigned int GetAddress() { return InstType != 2 ? ShortAddress : GetLongAddress(); }
		} Instruction;

	};

	// Returns contents of S1-reg field
	unsigned int RegS1() const { return Instruction.Reg1; }
	// Returns contents of S2-reg field (second S register index of type R instruction)
	unsigned int RegS2() const { return Instruction.Reg2; }
	// Returns contents of D-reg field for R type instructions (only)
	unsigned int RegD_R() const { return Instruction.ShortAddress / 4096; }//this might need to change
	// Returns contents of B-reg field for I type instructions (only)
	unsigned int RegB() const { return Instruction.Reg1; }
	// Returns contents of D-reg field for I type instructions (only)
	unsigned int RegD_I() const { return Instruction.Reg2; }
	// Returns contents of D-reg field for I and R instructions (based on first 2 bits only)
	unsigned int RegD() const { return (Instruction.InstType == 1) ? RegD_I() : RegD_R(); }
	// Returns contents of Reg 1 field for IO instructions only
	unsigned int RegR1() const { return Instruction.Reg1; }
	// Returns contents of Reg 2 field for IO instructions only
	unsigned int RegR2() const { return Instruction.Reg2; }

	// Returns contents of the 16-bit address field for I type instructions (only)
	unsigned int Address16() const { return Instruction.ShortAddress; }

	// Returns contents of the 24-bit address field for J type instructions (only)
	unsigned int Address24() const { return Instruction.Reg1 * (65536 * 16) + Instruction.Reg2 * 65536 + Instruction.ShortAddress;; }

	// Asserts that an instruction type is R
	void AssertInstructionTypeR() { assert(Instruction.InstType == 0); }
	// Asserts that an instruction type is I
	void AssertInstructionTypeI() { assert(Instruction.InstType == 1); }
	// Asserts that an instruction type is J
	void AssertInstructionTypeJ() { assert(Instruction.InstType == 2); }
	// Asserts that an instruction type is I/O
	void AssertInstructionTypeIO() { assert(Instruction.InstType == 3); }

};




class PageStruct
{
public:
	int ProcessID;
	int FrameNum;
	// We need to add clock variable to keep track of the least recently used frame. 

	//bool function that returns if frame num is valid
};


class Memory {
	int Size;
	MemoryWord *memory;

	PageStruct *PageMap = NULL;
	int VirtualMemory = -1;
	int *FrameMap;

	std::mutex LockMutex;

public:
	Memory(int SIZE, int _VirtualMemory =-1); //constructor
	void Clear();
	int GetSize() { return Size; }

	// Assures a page is in memory (i.e. has a frame) 
	int MapToFrame(int PageNum, int ProcessID=-1, int PC=-1);

	void write(int index, MemoryWord t, int Offset, int PID = -1, int PC = -1);
	MemoryWord read(int index, int Offset, int PID = -1, int PC = -1);
	unsigned int readContents(int index, int Offset, int PID=-1, int PC=-1) { return read(index, Offset, PID, PC).Contents; }

	void clearEverything();

};


// See last example on http://peterforgacs.github.io/2017/06/25/Custom-C-Exceptions-For-Beginners/

class RavosMemoryProtectionException : public std::exception
{
public:
	RavosMemoryProtectionException(int _ProcessID, int _Page, int _PC) { ProcessID = _ProcessID; Page = _Page; PC = _PC-1; }

	   int ProcessID;
	   int Page;
	   int PC;
};

class RavosPagingFaultException : public std::exception
{
public:
	RavosPagingFaultException(int _ProcessID, int _Page, int _PC) { ProcessID = _ProcessID; Page = _Page; PC = _PC-1; }

	int ProcessID;
	int Page;
	int PC;
};


class RavosInvalidMemoryAddressException : public std::exception
{
public:
	RavosInvalidMemoryAddressException(int _ProcessID, int _Page, int _PC) { ProcessID = _ProcessID; Page = _Page; PC = _PC - 1; }

	int ProcessID;
	int Page;
	int PC;
};

