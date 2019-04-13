#include "pch.h"

MMU::MMU(OS *_theOS)
{
	theOS = _theOS;
	for (int i = 0; i < 1024 / 4; i++)
	{
		FrameTracker[i] = -1;
	}
}


MemoryWord MMU::ReadOrPageFault(int index, PCB* pcb)
{
	//PageStruct *page = pcb->
	//Check page fault


}


void MMU::WriteOrPageFault(int index, PCB* pcb)
{

}