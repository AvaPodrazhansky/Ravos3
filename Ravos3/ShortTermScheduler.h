#pragma once
#include "pch.h"

static int TOTAL_WAIT_TIME = 0;//used for overall averaging, not one CPU specific
static int TOTAL_JOBS_EXECUTED = 0 ;//used in metrics and CPUmetrics
static int TOTAL_COMPLETION_TIME = 0;//used for overall averaging, not one CPU specific 

class ShortTermScheduler
{
public:
	OS* theOS = NULL;
	ShortTermScheduler(OS *_theOS) { theOS = _theOS;}
	
	bool Start_the_CPU(int cpu);

	//Over writes process in RAM with NULL values
	void ClearOldProcessFromRAM(PCB* pcb);
	
	
	bool Dispatch(int CPU_Num);


	int getAverageWaitTime();

	int getAverageCompletionTime();

};

