#pragma once
#include "pch.h"

class ShortTermScheduler
{
public:
	OS* theOS = NULL;
	ShortTermScheduler(OS *_theOS) { theOS = _theOS; ShortTermScheduler::Dispatch();  }


	//This is for when a process is finished executing
	//bool ClearOldProcessFromRAM(PCB* pcb);

	//Wrtie new process to RAM
	//bool WriteNewProcessToRAM(PCB* pcb);

	//bool ShortTermScheduler::Dispatch(PCB *pcb);

	//Over writes process in RAM with NULL values
	void ClearOldProcessFromRAM(PCB* pcb);
	
	//while the ready queue is not empty, send jobs to CPU if IDLE and calls to clear 
	//that sent job from RAM once it's state is set to TERMINATED
	//returns true when ready queue is empty 
	bool Dispatch();
	
	//~ShortTermScheduler();
	//Dispatcher()
};

