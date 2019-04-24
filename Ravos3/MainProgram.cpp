// Ravos3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

void printDisk(OS *theOS)
{
	for (int i = 1; i <= theOS->m_PCB_Map.size(); i++)
	{
		std::cout << std::dec << "\n Job " << i << " - BINARY SECTION " << "\n";
			PCB* pcb = theOS->m_PCB_Map.at(i);
		for (int j = 0; j < pcb->getProgramSize() + 44; j++)
		{
			if (j == pcb->getProgramSize() + 20 + 12)
				std::cout << std::dec << "\n Job " << i << " - TEMP SECTION " << "\n";
			else if (j == pcb->getProgramSize() + 20)
				std::cout << std::dec << "\n Job " << i << " - OUTPUT SECTION " << "\n";
			else if (j == pcb->getProgramSize())
				std::cout << std::dec << "\n Job " << i << " - INPUT SECTION " << "\n";
			printf("%08X \n", theOS->m_Computer->m_Disk.readContents(j, pcb->getStartIndexDisk()));
		}
	}
}

void writeCoreDump(OS *theOS) 
{
	std::ofstream outfile("CoreDump.txt");
	outfile << "CORE DUMP of ALL TASKS" << "\n";
	for (int i = 1; i <= theOS->m_PCB_Map.size(); i++)
	{
		outfile << std::dec << "\n Job " << i << " - BINARY SECTION " << "\n";
		PCB* pcb = theOS->m_PCB_Map.at(i);
		for (int j = 0; j < pcb->getProgramSize() + 44; j++)
		{
			if (j == pcb->getProgramSize() + 20 + 12)
				outfile << std::dec << "\n Job " << i << " - TEMP SECTION " << "\n";
			else if (j == pcb->getProgramSize() + 20)
				outfile << std::dec << "\n Job " << i << " - OUTPUT SECTION " << "\n";
			else if (j == pcb->getProgramSize())
				outfile << std::dec << "\n Job " << i << " - INPUT SECTION " << "\n";
			outfile  << std::setfill('0') << std::setw(8) << std::hex << (theOS->m_Computer->m_Disk.readContents(j, pcb->getStartIndexDisk())) << "\n";
		}
	}
	outfile.close();
}

void printMetrics(OS *theOS) 
{
	int avgWaitTime = 0;
	int avgCompTime = 0;
	int avgPageFault = 0;
	std::cout << " JOB METRICS " << "\n";
	for (int i = 1; i <= theOS->m_PCB_Map.size(); i++)
	{
		std::cout << std::dec << "\n Job " << i << " - Metrics " << "\n";
		PCB* pcb = theOS->m_PCB_Map.at(i);

		std::cout << "  Wait Time : " << pcb->getWaitTime() << " instructions \n";
		avgWaitTime += pcb->getWaitTime();
		std::cout << "  Interupts : " << pcb->getIOCount() << "\n";
		//outfile << "  Interupts : " << pcb->getInterupts();
		std::cout << "  Page Faults : " << pcb->getPageFaults() << "\n";
		avgPageFault += pcb->getPageFaults();
		std::cout << "  Completion Time : " << pcb->getCompletionTime() << " sec \n";
		avgCompTime += pcb->getCompletionTime();
		std::cout << "  Percent of RAM used : " << pcb->getPercentOfRAMUsed() << " % \n";
		std::cout << "  Percent of Cache used : " << pcb->getPercentOfCacheUsed() << " % \n";
	}
	std::cout << "\n Overall Averages " << "\n";
	std::cout << " Average Wait Time : " << (avgWaitTime / 30) << " instructions \n";
	std::cout << " Average Completion Time : " << (avgCompTime / 30) << " sec \n";
	std::cout << " Average Page Faults : " << (avgPageFault / 30) << " faults \n";
	std::cout << "\n CPU METRICS " << "\n";
	for (int j = 0; j < theOS->m_Computer->m_CPUMetrics_Map.size(); j++)
	{
		std::cout << std::dec << "\n CPU " << j << " - Metrics " << "\n";
		CPUMetrics* cpuMet = theOS->m_Computer->m_CPUMetrics_Map.at(j);

		std::cout << "  Number of Jobs completed : " << cpuMet->getNumOfJobs() << "\n";
		std::cout << "  Jobs CPU " << j << " executed : " << "\n";
		std::vector<int> jobsInThisCPU = cpuMet->getWhichJobsInThisCPU();
		for (int i : jobsInThisCPU)
		{
			std::cout << "    " << i << "\n";
		}
		std::cout << "  Percent of Jobs CPU " << j << " executed : " << cpuMet->getPercentOfJobsAssigned() << " % \n";
	}
}
void writeMetrics(OS *theOS) 
{
	int avgWaitTime = 0;
	int avgCompTime = 0;
	int avgPageFault = 0;
	std::ofstream outfiles("Metrics.txt");
	outfiles << " JOB METRICS " << "\n";
	for (int i = 1; i <= theOS->m_PCB_Map.size(); i++) 
	{
		outfiles << std::dec << "\n Job " << i << " - Metrics " << "\n";
		PCB* pcb = theOS->m_PCB_Map.at(i);

		outfiles << "  Wait Time : " << pcb->getWaitTime() << "\n";
		avgWaitTime += pcb->getWaitTime();
		outfiles << "  Interupts : " << pcb->getIOCount() << "\n";
		//outfile << "  Interupts : " << pcb->getInterupts();
		outfiles << "  Page Faults : " << pcb->getPageFaults() << "\n";
		avgPageFault += pcb->getPageFaults();
		outfiles << "  Completion Time : " << pcb->getCompletionTime() << "\n";
		avgCompTime += pcb->getCompletionTime();
		outfiles << "  Percent of RAM used : " << pcb->getPercentOfRAMUsed() << "\n";
		outfiles << "  Percent of Cache used : " << pcb->getPercentOfCacheUsed() << "\n";
	}
	outfiles << "\n Overall Averages " << "\n";
	outfiles << " Average Wait Time : " << (avgWaitTime/30)  << "\n";
	outfiles << " Average Completion Time : " << (avgCompTime/30) << "\n";
	outfiles << " Average Page Faults : " << (avgPageFault / 30) << " faults \n";
	outfiles << "\n CPU METRICS " << "\n";
	for (int j = 0; j < theOS->m_Computer->m_CPUMetrics_Map.size(); j++) 
	{
		outfiles << std::dec << "\n CPU " << j << " - Metrics " << "\n";
		CPUMetrics* cpuMet = theOS->m_Computer->m_CPUMetrics_Map.at(j);

		outfiles << "  Number of Jobs completed : " << cpuMet->getNumOfJobs() << "\n";
		outfiles << "  Jobs CPU " << j << " executed : " << "\n";
		std::vector<int> jobsInThisCPU = cpuMet->getWhichJobsInThisCPU();
		for (int i : jobsInThisCPU) 
		{
			outfiles << "    " << i << "\n";
		}
		outfiles << "  Percent of Jobs CPU " << j << " executed : " << cpuMet->getPercentOfJobsAssigned();
	}
	outfiles.close();
}

//This is the Driver 
int main()
{
	int rerun = 1;
	do
	{
		int NumOfCPU = 0;
		do
		{
			std::cout << "Enter Number of CPU's: (1 - 4) ";
			std::cin >> NumOfCPU;
			if (NumOfCPU > 4)
				std::cout << "Too many CPUs\n";
			if (NumOfCPU < 1)
				std::cout << "Must have at least 1 CPU to run\n";
		} while (NumOfCPU > 4 || NumOfCPU < 1);
		std::cout << "Enter Scheduling Algorithm (1 = FIFO, 2 = SJF, 3 = Priority) ";
		int SchedAlgor;
		std::cin >> SchedAlgor;


		Computer *theComputer = new Computer(NumOfCPU);
		OS theOS(theComputer);

	   //Calls loader
		if (!theOS.Boot("programfile.txt"))
		{
			std::cout << "OS Could not boot\n";
			return 0;
		}

		switch (SchedAlgor)
		{
		case 1: theOS.m_Scheduler.SchedType = FIFO;
			break;
		case 2: theOS.m_Scheduler.SchedType = SJF;
			break;
		case 3: theOS.m_Scheduler.SchedType = PRIORITY;
		}
		theOS.m_Scheduler.FillJobQueue();
		theOS.m_Scheduler.FillReadyQueue();

		while (!theOS.m_ReadyQueue.empty() || !theOS.m_Scheduler.m_JobQueue.empty())
		{
			while (!theOS.m_ReadyQueue.empty())
			{
				using namespace std::literals::chrono_literals;

				for (int i = 0; i < NumOfCPU; i++)
				{
					if (!theOS.m_ReadyQueue.empty())
					{
						if (theComputer->m_CPU[i].GetState() == IDLE)
						{
							theOS.m_ShortTerm.Dispatch(i);
							theOS.m_Computer->m_CPU[i].m_thread_ptr = new std::thread(&CPU::CPU_Run_thread, theOS.m_Computer->m_CPU[i]);
						}
					}
				}
			}
		}

		// Join threads
		for (int c = 0; c < theComputer->GetNumCPUs(); ++c)
			if (theOS.m_Computer->m_CPU[c].m_thread_ptr->joinable())
				theOS.m_Computer->m_CPU[c].m_thread_ptr->join();


		std::cout << "Done! \n";


		OS *OS_Pointer = &theOS;
		writeCoreDump(OS_Pointer);
		writeMetrics(OS_Pointer);

		std::cout << "Print Core dump? (y/n)";
		char coreDump;
		std::cin >> coreDump;
		if (coreDump == 'y' || coreDump == 'Y')
		{
			printDisk(OS_Pointer);
		}
		std::cout << "Print Metrics? (y/n)";
		char metrics;
		std::cin >> metrics;
		if (metrics == 'y' || metrics == 'Y')
		{
			printMetrics(OS_Pointer);
		}
		int rerun;
		std::cout << "Enter 1 to restart OS: ";
		std::cin >> rerun;

	}while (rerun == 1);

}
	


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
