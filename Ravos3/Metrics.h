#pragma once
	static int TOTAL_WAIT_TIME = 0;//used for overall averaging, not one CPU specific
	static int TOTAL_JOBS_EXECUTED = 0;//used in metrics and CPUmetrics
	static int TOTAL_COMPLETION_TIME = 0;//used for overall averaging, not one CPU specific
class Metrics 
{
private:
	//job metrics
	//int WaitTime;//num of instructions before current 
	//int NumOfIO;//number of IO instructions per job
	//int interupts;//num of interupts, will be implemented later
	//int CompletionTime;
	int AverageWaitTime;
	int AverageCompletionTime;
	PCB *thePCB = NULL;

public:

	Metrics(PCB* _thePCB) { thePCB = _thePCB; setPercentofRAMPerProcess(); setPercentOfCachePerProcess(); }//called in loader

	//metrics that only depend on the original job information 
	bool setPercentofRAMPerProcess();

	bool setPercentOfCachePerProcess();
	
	

	//bool updateWaitTime(PCB* tempPCB); //call in dispatcher, will call updateTotalWaitTime after

	bool updateTotalWaitTime(PCB* tempPCB); //called in updateWaitTime

	bool updateIOCount(PCB* tempPCB);//call in execute after RD and WR

	bool setCompletionTime(PCB* tempPCB, int CTime);//called in CPU.cpp after HLT reached


	int getAverageWaitTime();

	int getAverageCompletionTime();


};