#pragma once
	static int TOTAL_WAIT_TIME;//used for overall averaging, not one CPU specific
	static int TOTAL_JOBS_EXECUTED;//used in metrics and CPUmetrics
	static int TOTAL_COMPLETION_TIME;//used for overall averaging, not one CPU specific
class Metrics 
{
private:
	//job metrics
	int WaitTime;//num of instructions before current 
	int NumOfIO;//number of IO instructions per job
	int interupts;//num of interupts, will be implemented later
	int CompletionTime;
	int AverageWaitTime;


	Metrics(PCB* _pcb) { pcb = _pcb; }//keeps track of Job metrics

	bool updateTotalWaitTime(PCB* tempPCB); //called after program has been executed

	bool updateWaitTime(); //call in dispatcher

	bool updateIOCount();//call in execute after RD and WR

	//
	bool setCompletionTime() 
	{

	}

	int getAverageWaitTime();




	PCB* pcb = NULL;
};