#pragma once
#include "pch.h"


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
	int RegS1() const { return Instruction.Reg1; }
	// Returns contents of S2-reg field (second S register index of type R instruction)
	int RegS2() const { return Instruction.Reg2; }
	// Returns contents of D-reg field for R type instructions (only)
	int RegD_R() const { return Instruction.ShortAddress / 4096; }//this might need to change
	// Returns contents of B-reg field for I type instructions (only)
	int RegB() const { return Instruction.Reg1; }
	// Returns contents of D-reg field for I type instructions (only)
	int RegD_I() const { return Instruction.Reg2; }
	// Returns contents of D-reg field for I and R instructions (based on first 2 bits only)
	int RegD() const { return (Instruction.InstType == 1) ? RegD_I() : RegD_R(); }
	// Returns contents of Reg 1 field for IO instructions only
	int RegR1() const { return Instruction.Reg1; }
	// Returns contents of Reg 2 field for IO instructions only
	int RegR2() const { return Instruction.Reg2; }

	// Returns contents of the 16-bit address field for I type instructions (only)
	int Address16() const { return Instruction.ShortAddress; }

	// Returns contents of the 24-bit address field for J type instructions (only)
	int Address24() const { return Instruction.Reg1 * (65536 * 16) + Instruction.Reg2 * 65536 + Instruction.ShortAddress;; }

	// Asserts that an instruction type is R
	void AssertInstructionTypeR() { assert(Instruction.InstType == 0); }
	// Asserts that an instruction type is I
	void AssertInstructionTypeI() { assert(Instruction.InstType == 1); }
	// Asserts that an instruction type is J
	void AssertInstructionTypeJ() { assert(Instruction.InstType == 2); }
	// Asserts that an instruction type is I/O
	void AssertInstructionTypeIO() { assert(Instruction.InstType == 3); }

};




class Memory {
	int Size;
	MemoryWord *memory;

public:
	Memory(int SIZE); //constructor
	void Clear();
	int GetSize() { return Size; }

	void write(int index, MemoryWord t);
	MemoryWord read(int index);
	unsigned int readContents(int index) { return read(index).Contents; }


};

