#include "pch.h"


bool Metrics::updateTotalWaitTime(PCB* tempPCB) //called after program has been executed
{
	TOTAL_WAIT_TIME += tempPCB->getProgramSize();
	return true;
}

bool Metrics::updateWaitTime() //call in dispatcher
{
	WaitTime = TOTAL_WAIT_TIME;
	return true;
}

bool Metrics::updateIOCount()
{
	NumOfIO++;
	return true;
}
int Metrics::getAverageWaitTime()
{
	AverageWaitTime = TOTAL_WAIT_TIME / TOTAL_JOBS_EXECUTED;
	return AverageWaitTime;
}