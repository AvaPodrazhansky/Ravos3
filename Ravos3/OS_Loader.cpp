#include "pch.h"

bool OS::Load(const char *Filename)
{
	// TODO: Implement the Loader.  Put each program on disk and create a PCB for them.  Return true if load successful

	return false;
}




//// Just for testing
//void ReadProgString(Memory &m, const char *s)
//{
//	int size = strlen(s);
//	int Loc = 0;
//
//	for (int i = 0; i + 8 < size; i += 8)
//	{
//		//		if (s[i] == '\n') ++i;
//		MemoryWord w;
//
//		for (int j = 0; j < 8; ++j)
//			w.Contents[j] = s[i + j];
//
//		m.write(Loc++, w);
//	}
//
//
//}
//



const char* Prog1 =
"C050005C"
"4B060000"
"4B010000"
"4B000000"
"4F0A005C"
"4F0D00DC"
"4C0A0004"
"C0BA0000"
"42BD0000"
"4C0D0004"
"4C060001"
"10658000"
"56810018"
"4B060000"
"4F0900DC"
"43970000"
"05070000"
"4C060001"
"4C090004"
"10658000"
"5681003C"
"C10000AC"
"92000000";

/*
// Data 14 C C

0x0000000A

0x00000006

0x0000002C

0x00000045

0x00000001

0x00000007

0x00000000

0x00000001

0x00000005

0x0000000A

0x00000055

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

0x00000000

// END

*/


void OS::TestLoad1()
{
	PCB *c = new PCB();
	c->process_ID = 1;
	c->priority = 2;
	c->ProgramSize = 17;

	//  pcb->InputSize  = ??
	//  pcb->OutputSize = ??
	//  pcb->TempSize    = ??

//	ReadProgString(m_Computer->m_RAM, Prog1);

	// Something like this...
//	m_PCB.insert(c->getProcessID, c);



}


