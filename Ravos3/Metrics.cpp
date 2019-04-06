#include "pch.h"
//
//bool Metrics::setPercentofRAMPerProcess()
//{
//	int RAMpercent = (thePCB->getProgramSize() / 1024);//will use MAX_RAM later
//	thePCB->setPercentOfRAMUsed(RAMpercent);
//	return true;
//}
//
//bool Metrics::setPercentOfCachePerProcess()
//{
//	int CachePercent = (thePCB->getProgramSize() / 28);//will use MAX_CACHE later
//	thePCB->setPercentOfCacheUsed(CachePercent);
//	return true;
//}
//
//
//
////
////bool Metrics::updateWaitTime(PCB* tempPCB) //call in dispatcher
////{
////	tempPCB->setWaitTime(TOTAL_WAIT_TIME);
////	Metrics::updateTotalWaitTime(tempPCB);
////	return true;
////}
//bool Metrics::updateTotalWaitTime(PCB* tempPCB)
//{
//	TOTAL_WAIT_TIME += tempPCB->getProgramSize();
//	return true;
//}
//
//bool Metrics::updateIOCount(PCB* tempPCB)
//{
//	tempPCB->setIOCount(tempPCB->getIOCount() + 1);
//	return true;
//}
//
//bool Metrics::setCompletionTime(PCB* tempPCB, int CTime)//called in CPU.cpp after HLT reached
//{
//	tempPCB->setCompletionTime(CTime);
//	return true;
//}
//
//int Metrics::getAverageWaitTime()
//{
//	AverageWaitTime = TOTAL_WAIT_TIME / TOTAL_JOBS_EXECUTED;
//	return AverageWaitTime;
//}
//
//int Metrics::getAverageCompletionTime() 
//{
//	AverageCompletionTime = TOTAL_COMPLETION_TIME / TOTAL_JOBS_EXECUTED;
//	return AverageCompletionTime;
//}