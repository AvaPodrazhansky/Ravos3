#pragma once


// TODO: Design scheduler.  First we'll do the simplest long-term Dispatcher.  Afterwards we'll improve

//TODO: Add enum for scheduler types: FIFO, SJF, Priority (This is for Phase 2)

class Scheduler
{
public:
	// queue ReadyQueue
	// 


	// Dispatches programs that are to be run on the various CPUs. 
	void Dispatch();

	//bool WriteFromDiskToRam();

	//Returns the PID of next program to be executed.
	int FIFOScheduler();

	int SJFScheduler();

	int PriorityScheduler();
};