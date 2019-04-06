#pragma once
enum State 
{
	New = 0, Ready = 1, Running = 2, Waiting = 3, Terminated = 4
};

extern int TOTAL_WAIT_TIME;//used for overall averaging, not one CPU specific
extern int TOTAL_JOBS_EXECUTED;//used in metrics and CPUmetrics
extern int TOTAL_COMPLETION_TIME;//used for overall averaging, not one CPU specific 

class PCB
{
	friend class OS;
	friend class CPU;
	friend class Scheduler;
	friend class ShortTermScheduler;
private:

	int process_ID;
	int priority;	//-20 to 19
	State state = New;	//[0 = New, 1 = Ready, 2 = Running, 3 = Waiting, 4 = Terminated]

	//int arrived;	//Do we still need this since state has been expanded?
	//int length; using progam size instead
	//int start;	//Same question as above
	//int wait;    	//Also same

	int ProgramSize;  // Length of the program
	int StartIndexDisk;
	int StartIndexRAM;

	int InputBufferSize;
	int InputBufferStart;  // Location on disk where it starts
	int InputBufferNext;   // Location on disk for next read, keeps track of where you are in buffer

	int	OutputBufferSize;
	int OutputBufferStart;
	int OutputBufferNext;

	int TempBufferSize;
	int TempBufferStart;
	int TempBufferNext;

	int WaitTime;//num of instructions before current 
	int IOCount;//number of IO instructions per job
	int interupts;//num of interupts, will be implemented later
	int CompletionTime;//how long a process takes in go through all it's instructions in milliseconds
	double PercentOfRAMUsed;//percent of RAM that will be used for this process
	double PercentOfCacheUsed;//percent of Cache that will be used for this process
	int AverageWaitTime;


public:
//	Metrics m_Metrics;
	PCB() 
	{
		priority = 19;
		state = New;
		//arrived = 0;	//Need?
		ProgramSize = 0;		//Need?
		IOCount = 0;
	}


	/*Creates a PCB with the specified parameters.
	 * @param arr the cycle during which the process arrived
	 * @param len the quantum - number of cycles required to execute
	 * the process
	 */
	PCB(int ID, int priority_Val, int arr, int len)
	{
		process_ID = ID;
		priority = priority_Val;
		//arrived = arr;
		ProgramSize = len;
		IOCount = 0;
	}

	int getProcessID() const
	{
		return process_ID;
	}

	int getPriority() const
	{
		return priority;
	}

	int getProgramSize() const
	{
		return ProgramSize;
	}

	bool isExecuting() const
	{
		return (state == Running);
	}

	void setProcessID(int tp) 
	{
		process_ID = tp;
	}

	void setPriority(int tpp)
	{
		priority = tpp;
	}

	void setProgramSize(int tps) 
	{
		ProgramSize = tps;
	}

	void setInputBufferSize(int tbuff) 
	{
		InputBufferSize = tbuff;
	}
	void setInputBufferStart(int tstart) 
	{
		InputBufferStart = tstart;
	}
	void setOutputBufferSize(int tbuff) 
	{
		OutputBufferSize = tbuff;
	}
	void setOutputBufferStart(int tstart) 
	{
		OutputBufferStart = tstart;
	}
	void setTempBufferSize(int tbuff) 
	{
		TempBufferSize = tbuff;
	}
	void setTempBufferStart(int tstart) 
	{
		TempBufferStart = tstart;
	}

	void setStartIndexRAM(int RAMsi) 
	{
		StartIndexRAM = RAMsi;
	}

	int getStartIndexRAM() 
	{
		return StartIndexRAM;
	}

	int getStartIndexDisk() 
	{
		return StartIndexDisk;
	}

	int totalSpaceInRAM()
	{
		return ProgramSize + InputBufferSize + OutputBufferSize + TempBufferSize; //might need to add output buffer depending on what all is being written to RAM
	}


	//metrics value setters and getters

	//called at end of loader
	void setPercentofRAMPerProcess()
	{
		double RAMpercent = ((static_cast<double>(ProgramSize) / 1024) * 100);//will use MAX_RAM later
		PercentOfRAMUsed = RAMpercent;
	}

	void setPercentOfCachePerProcess()
	{
		double CachePercent = ((static_cast<double>(ProgramSize) / 28) * 100);//will use MAX_CACHE later
		PercentOfCacheUsed = CachePercent;
	}
	//called in execute() in CPU
	void setCompletionTime(int tempCompTime) 
	{
		CompletionTime = tempCompTime;
		TOTAL_COMPLETION_TIME += CompletionTime;
	}
	int getCompletionTime() 
	{
		return CompletionTime;
	}
	//called before dispatched to cpu 
	void setWaitTime()
	{
		WaitTime = TOTAL_WAIT_TIME;
		TOTAL_WAIT_TIME += ProgramSize;
	}

	int getIOCount() 
	{
		return IOCount;
	}
	//called after WR and RD in execute
	void updateIOCount() 
	{
		IOCount += 1;
	}

};
