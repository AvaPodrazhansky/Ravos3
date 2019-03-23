#pragma once


class PCB
{
	friend class OS;

private:

	int process_ID;
	int priority;	//-20 to 19
	int state;	//[0 = New, 1 = Ready, 2 = Running, 3 = Waiting, 4 = Terminated]

	int arrived;	//Do we still need this since state has been expanded?
	//int length; using progam size instead
	int start;	//Same question as above
	int wait;    	//Also same

	int ProgramSize;  // Length of the program
	int ProgramStartOnDisk;
	int ProgramStartInRAM;

	int InputBufferSize;
	int InputBufferStart;  // Location on disk where it starts
	int InputBufferNext;   // Location on disk for next read, keeps track of where you are in buffer

	int	OutputBufferSize;
	int OutputBufferStart;
	int OutputBufferNext;

	int TempBufferSize;
	int TempBufferStart;
	int TempBufferNext;

public:

	PCB()
	{
		priority = 19;
		state = 0;
		arrived = 0;	//Need?
		ProgramSize = 0;		//Need?
	}

	/*Creates a PCB with the specified parameters.
	 * @param arr the cycle during which the process arrived
	 * @param len the quantum - number of cycles required to execute
	 * the process
	 */
	PCB(int ID, int priority_Val, int status, int arr, int len)
	{
		process_ID = ID;
		priority = priority_Val;
		state = status;
		arrived = arr;
		ProgramSize = len;
	}

	int getProcessID() const
	{
		return process_ID;
	}

	int getPriority() const
	{
		return priority;
	}

	bool isExecuting() const
	{
		return (state == 2);
	}

	void execute()
	{
		state = 2;
	}

	void setWait(int waitTime)
	{
		wait = waitTime;
	}

	//determines whether two process control blocks are the same.
	friend bool operator==(const PCB& pcb1, const PCB& pcb2);

	//determines whether two process control blocks are different
	friend bool operator!=(const PCB& pcb1, const PCB& pcb2);

	//determines whether the first PCB has a higher priority than the second.   
	friend bool operator>(const PCB& pcb1, const PCB& pcb2);

	//determines whether the first PCB has a lower priority than the second  
	friend bool operator<(const PCB& pcb1, const PCB& pcb2);

	//determines whether the first PCB has the same or higher priority than the second    
	friend bool operator>=(const PCB& pcb1, const PCB& pcb2);

	// determines whether the first PCB has the same or lower priority than the second  
	friend bool operator<=(const PCB& pcb1, const PCB& pcb2);
};
