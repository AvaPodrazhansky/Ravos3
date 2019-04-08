// Ravos3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//#include <iostream>

//This is ther Driver for now
int main()
{
	Computer *theComputer = new Computer();
	OS theOS(theComputer);
	
	//Calls loader
	if (!theOS.Boot("programfile.txt"))
	{
		std::cout << "OS Could not boot\n";
		return 0;
	}

	std::cout << "Ready to run\n";
	//theOS.Schedule_and_Run();
 	printf("%5s %5s %9s %9s\n", "CPU","Job", "Priority", "Length");

	theOS.m_Scheduler.FillJobQueue();

	theOS.m_Scheduler.FillReadyQueue();

	//while (!theOS.m_ReadyQueue.empty())
	//{
	//	using namespace std::literals::chrono_literals;

	//	while (theOS.m_Computer->m_CPU[0].m_C_State == BUSY)
	//	{
	//		std::this_thread::sleep_for(1s);
	//	}

	//	theOS.m_ShortTerm.Dispatch(0);
	//	theComputer->m_CPU[0].CPU_Run_thread();
	//}

	while (!theOS.m_ReadyQueue.empty())
	{
		using namespace std::literals::chrono_literals;
		
		for (int i = 0; i < theComputer->GetNumCPUs(); i++)
		{
			if (theComputer->m_CPU[i].m_C_State == IDLE)
			{
				theOS.m_ShortTerm.Dispatch(i);
				theOS.m_Computer->m_CPU[i].m_C_State = BUSY;
				//theOS.busyCPUs++;
				theComputer->m_CPU[i].CPU_Run_thread();
			}
		}
	}

	

//	while (!theOS.allJobsExecuted()) {}

	//theOS.m_ShortTerm.Start_the_CPU();

	//theOS.m_Scheduler.FillJobQueue();


//	while (true) {}// { std::cout << "Main thread is running\n"; }
}
	//do {
	//	//Call Long Term Scheudler 
	//	if (!theOS.m_Scheduler.FIFOScheduler())
	//	{
	//		std::cout << "Long Term Scheduler Error\n";
	//	}

	//	//Calls Short Term Scheduler
	//	if (!theOS.m_ShortTerm.Dispatch())
	//	{
	//		std::cout << "Unable to dispatch\n";
	//	}

	//} while (!theOS.m_Scheduler.m_JobQueue.empty());


	//std::cout << "FIFO Test\n";
	//printf("%5s %9s %9s\n", "Job", "Priority", "Length");
	//	do {
	//	//Call Long Term Scheudler 
	//	if (!theOS.m_Scheduler.FillJobQueue())
	//	{
	//		std::cout << "Long Term Scheduler Error\n";
	//	}

	//	//Calls Short Term Scheduler
	//	if (!theOS.m_ShortTerm.Dispatch())
	//	{
	//		std::cout << "Unable to dispatch\n";
	//	}

	//} while (!theOS.m_Scheduler.m_JobQueue.empty());

	//Test Priority
	//std::cout << "Priority Test\n";
	//printf("%5s %9s %9s\n", "Job", "Priority", "Length");
	//theOS.m_Scheduler.SchedType = PRIORITY;
	//do {
	//	//Call Long Term Scheudler 
	//	if (!theOS.m_Scheduler.FillJobQueue())
	//	{
	//		std::cout << "Long Term Scheduler Error\n";
	//	}

	//	//Calls Short Term Scheduler
	//	if (!theOS.m_ShortTerm.Dispatch())
	//	{
	//		std::cout << "Unable to dispatch\n";
	//	}

	//} while (!theOS.m_Scheduler.m_JobQueue.empty());

//		//Test SJF
//	std::cout << "SJF Test\n";
//	printf("%5s %9s %9s\n", "Job", "Priority", "Length");
//	theOS.m_Scheduler.SchedType = SJF;
//	do {
//		//Call Long Term Scheudler 
//		if (!theOS.m_Scheduler.FillJobQueue())
//		{
//			std::cout << "Long Term Scheduler Error\n";
//		}
//
//		//Calls Short Term Scheduler
//		if (!theOS.m_ShortTerm.Dispatch(0))
//		{
//			std::cout << "Unable to dispatch\n";
//		}
//
//	} while (!theOS.m_Scheduler.m_JobQueue.empty());
//	std::cout << "Done!\n";
//
//}




//test the metrics


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
