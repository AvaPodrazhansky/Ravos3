#pragma once
#include "pch.h"

class ShortTermScheduler
{
public:
	OS* theOS = NULL;
	ShortTermScheduler(OS *_theOS);

	//This is for when a process is finished executing
	bool ClearOldProcessFromRAM(PCB* pcb);

	//Wrtie new process to RAM
	bool WriteNewProcessToRAM(PCB* pcb);

	bool Dispatch(PCB *pcb);
	
	//~ShortTermScheduler();
	//Dispatcher()
};

