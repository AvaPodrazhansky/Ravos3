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

		//Test SJF
	std::cout << "Priority Test\n";
	printf("%5s %9s %9s\n", "Job", "Priority", "Length");
	theOS.m_Scheduler.SchedType = SJF;
	do {
		//Call Long Term Scheudler 
		if (!theOS.m_Scheduler.FillJobQueue())
		{
			std::cout << "Long Term Scheduler Error\n";
		}

		//Calls Short Term Scheduler
		if (!theOS.m_ShortTerm.Dispatch())
		{
			std::cout << "Unable to dispatch\n";
		}

	} while (!theOS.m_Scheduler.m_JobQueue.empty());
	std::cout << "Done!\n";

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
