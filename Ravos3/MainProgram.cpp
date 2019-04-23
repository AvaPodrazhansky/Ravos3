// Ravos3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//#include <iostream>

//void printDisk(OS *theOS)
//{
//	for (int i = 1; i <= theOS->m_PCB_Map.size(); i++)
//	{
//		std::cout << std::dec << "\n* Job " << i << "*\n";
//		PCB* pcb = theOS->m_PCB_Map.at(i);
//		for (int j = 0; j < pcb->getProgramSize() + 44; j++)
//		{
//			if (j == pcb->getProgramSize() + 20 + 12)
//				std::cout << "* Temp *\n";
//			else if (j == pcb->getProgramSize() + 20)
//				std::cout << "* Out  *\n";
//			else if (j == pcb->getProgramSize())
//				std::cout << "* Inpt *\n";
//			printf("%08X \n", theOS->m_Computer->m_Disk.readContents(j, pcb->getStartIndexDisk()));
//		}
//	}
//}
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
		std::cout << "  Completion Time : " << pcb->getCompletionTime() << " sec \n";
		avgCompTime += pcb->getCompletionTime();
		std::cout << "  Percent of RAM used : " << pcb->getPercentOfRAMUsed() << " % \n";
		std::cout << "  Percent of Cache used : " << pcb->getPercentOfCacheUsed() << " % \n";
	}
	std::cout << "\n Overall Averages " << "\n";
	std::cout << " Average Wait Time : " << (avgWaitTime / 30) << " instructions \n";
	std::cout << " Average Completion Time : " << (avgCompTime / 30) << " sec \n";
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
		outfiles << "  Completion Time : " << pcb->getCompletionTime() << "\n";
		avgCompTime += pcb->getCompletionTime();
		outfiles << "  Percent of RAM used : " << pcb->getPercentOfRAMUsed() << "\n";
		outfiles << "  Percent of Cache used : " << pcb->getPercentOfCacheUsed() << "\n";
	}
	outfiles << "\n Overall Averages " << "\n";
	outfiles << " Average Wait Time : " << (avgWaitTime/30)  << "\n";
	outfiles << " Average Completion Time : " << (avgCompTime/30) << "\n";
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
////This is ther Driver for now
//int main()
//{
//	Computer *theComputer = new Computer();
//	OS theOS(theComputer);
//
//	//Calls loader
//	if (!theOS.Boot("programfile.txt"))
//	{
//		std::cout << "OS Could not boot\n";
//		return 0;
//	}
//
//	std::cout << "Ready to run\n";
//	/*
//		//printf("%5s %5s %9s %9s\n", "CPU", "Job", "Priority", "Length");
//
//		theOS.m_Scheduler.FillJobQueue();
//
//		theOS.m_Scheduler.FillReadyQueue();
//
//		//theOS.Schedule_and_Run();
//		//std::queue <PCB*> writeQueue;
//
//		while (!theOS.m_ReadyQueue.empty() || !theOS.m_Scheduler.m_JobQueue.empty())
//		{
//			while (!theOS.m_ReadyQueue.empty())
//			{
//				using namespace std::literals::chrono_literals;
//
//				for (int i = 0; i < theComputer->GetNumCPUs(); i++)
//				{
//					if (!theOS.m_ReadyQueue.empty())
//					{
//						if (theComputer->m_CPU[i].GetState() == IDLE)
//						{
//							//writeQueue.push(theOS.m_ReadyQueue.front());
//							theOS.m_ShortTerm.Dispatch(i);
//							theOS.m_Computer->m_CPU[i].m_thread_ptr = new std::thread(&CPU::CPU_Run_thread, theOS.m_Computer->m_CPU[i]);
//						}
//					}
//				}
//			}
//
//			//for (int c = 0; c < theComputer->GetNumCPUs(); ++c)
//			//	if (theOS.m_Computer->m_CPU[c].m_thread_ptr->joinable())
//			//		theOS.m_Computer->m_CPU[c].m_thread_ptr->join();
//
//			//break;  // We do this to ensure that we complete loop, until the following part is coded.
//
//			if (!theOS.m_Scheduler.m_JobQueue.empty())
//			{
//
//				for (int c = 0; c < theComputer->GetNumCPUs(); ++c)
//					if (theOS.m_Computer->m_CPU[c].m_thread_ptr->joinable())
//						theOS.m_Computer->m_CPU[c].m_thread_ptr->join();
//
//				// At this point, RAM can be wiped.
//				theOS.m_Computer->m_RAM.clearEverything();
//
//				// Move programs in job queue to ram (you may have to re-base (i.e. start at location 0 again)
//				// then fill ready queue again: theOS.m_Scheduler.FillReadyQueue();
//				theOS.m_Scheduler.FillReadyQueue();
//
//			}
//		}
//
//		for (int i = 1; i <= theOS.m_PCB_Map.size(); i++)
//		{
//			std::cout << std::dec << "Job " << i << "\n";
//			PCB* pcb = theOS.m_PCB_Map.at(i);
//			for(int j=0; j<pcb->getProgramSize() + 44; j++)
//			std::cout << std::hex << theOS.m_Computer->m_Disk.readContents(j, pcb->getStartIndexDisk(), -2, -2) << "\n";
//		}
//		std::cout << "Done!" << std::endl;
//		*/
//
//		/*
//		//PCB* pcb1 = theOS.m_PCB_Map.at(1);
//		//PCB* pcb2 = theOS.m_PCB_Map.at(2);
//
//		//for (int i = 0; i < 8; i++)
//		//{
//		//	theOS.m_MMU.ReadOrPageFault(i, pcb1);
//		//}
//		//
//		//for (int i = 0; i < 8; i++)
//		//{
//		//	theOS.m_MMU.ReadOrPageFault(i, pcb2);
//		//}
//
//		//theOS.m_MMU.WriteOrPageFault(4, MemoryWord(0xFFFFFFFF), pcb1);
//		//theOS.m_MMU.WriteOrPageFault(9, MemoryWord(0xAAAAAAAA), pcb1);
//		//theOS.m_MMU.WriteOrPageFault(2, MemoryWord(0xBBBBBBBB), pcb2);
//		//theOS.m_MMU.WriteOrPageFault(31, MemoryWord(0xCCCCCCCC), pcb2);
//		//theOS.m_MMU.printValidFrames();
//		//theOS.m_MMU.RemovePagesFromRAM(pcb1);
//		//for (int i = 0; i < 63; i++)
//		//{
//		//	std::cout << std::dec << i << " " << std::hex << theOS.m_Computer->m_Disk.read(i, 0).Contents << "\n";
//		//}
//		//
//		//theOS.m_MMU.printValidFrames();
//		*/
//		/*
//		//for (int i = 1; i <= 16; i++)
//		//{
//		//	printf("%4s %10s %5s %5s %5s\n", "Job", "Instruction", "Page", "Frame", "Index");
//		//	PCB* pcb1 = theOS.m_PCB_Map.at(i);
//		//	for (int i = 0; i < pcb1->totalSpaceInRAM(); i++)
//		//		{
//		//			theOS.m_MMU.ReadOrPageFault(i, pcb1);
//		//		}
//		//	theOS.m_MMU.printValidFrames();
//		//}
//		*/
//		
//			theOS.m_Scheduler.SchedType = FIFO;
//			//theOS.m_MMU.ManageMemory = true;
//			theOS.m_Scheduler.FillJobQueue();
//			theOS.m_Scheduler.FillReadyQueue();
//
//			while (!theOS.m_ReadyQueue.empty() || !theOS.m_Scheduler.m_JobQueue.empty())
//			{
//				while (!theOS.m_ReadyQueue.empty())
//				{
//					using namespace std::literals::chrono_literals;
//
//					for (int i = 0; i < theComputer->GetNumCPUs(); i++)
//					{
//						if (!theOS.m_ReadyQueue.empty())
//						{
//							if (theComputer->m_CPU[i].GetState() == IDLE)
//							{
//								//writeQueue.push(theOS.m_ReadyQueue.front());
//								theOS.m_ShortTerm.Dispatch(i);
//								theOS.m_Computer->m_CPU[i].m_thread_ptr = new std::thread(&CPU::CPU_Run_thread, theOS.m_Computer->m_CPU[i]);
//							}
//						}
//					}
//					/*for (int c = 0; c < theComputer->GetNumCPUs(); ++c)
//						if (theOS.m_Computer->m_CPU[c].m_thread_ptr->joinable())
//							theOS.m_Computer->m_CPU[c].m_thread_ptr->join();*/
//				}
//			}
//			
//
//	//PCB* pcb1 = theOS.m_PCB_Map.at(1);
//	//theOS.m_ReadyQueue.push(pcb1);
//	//theOS.m_ShortTerm.Dispatch(0);
//	//theOS.m_Computer->m_CPU[0].m_thread_ptr = new std::thread(&CPU::CPU_Run_thread, theOS.m_Computer->m_CPU[0]);
//
//
//	for (int c = 0; c < theComputer->GetNumCPUs(); ++c)
//		if (theOS.m_Computer->m_CPU[c].m_thread_ptr->joinable())
//			theOS.m_Computer->m_CPU[c].m_thread_ptr->join();
//
//	OS *OS_Pointer = &theOS;
//	printDisk(OS_Pointer);
//}

//This is ther Driver for now
int main()
{
	int rerun = 1;
	//std::cout << "Enter 1 to start OS: ";
	//std::cin >> rerun;
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
		//std::cout << NumOfCPU;
		std::cout << "Enter Scheduling Algorithm (1 = FIFO, 2 = SJF, 3 = Priority) ";
		int SchedAlgor;
		std::cin >> SchedAlgor;
		//std::cout << "Paging or not paging? (y/n) ";
		//char paging;
		//std::cin >> paging;

		Computer *theComputer = new Computer(NumOfCPU);//will need to figure out how to change the array value
		/*if(paging == 'y')*/

		OS theOS(theComputer);

		/*else
			OS theOS(theComputer, false);
	*/
	//Calls loader
		if (!theOS.Boot("programfile.txt"))
		{
			std::cout << "OS Could not boot\n";
			return 0;
		}


		/*
		//PCB* pcb1 = theOS.m_PCB_Map.at(1);
		//PCB* pcb2 = theOS.m_PCB_Map.at(2);

		//for (int i = 0; i < 8; i++)
		//{
		//	theOS.m_MMU.ReadOrPageFault(i, pcb1);
		//}
		//
		//for (int i = 0; i < 8; i++)
		//{
		//	theOS.m_MMU.ReadOrPageFault(i, pcb2);
		//}

		//theOS.m_MMU.WriteOrPageFault(4, MemoryWord(0xFFFFFFFF), pcb1);
		//theOS.m_MMU.WriteOrPageFault(9, MemoryWord(0xAAAAAAAA), pcb1);
		//theOS.m_MMU.WriteOrPageFault(2, MemoryWord(0xBBBBBBBB), pcb2);
		//theOS.m_MMU.WriteOrPageFault(31, MemoryWord(0xCCCCCCCC), pcb2);
		//theOS.m_MMU.printValidFrames();
		//theOS.m_MMU.RemovePagesFromRAM(pcb1);
		//for (int i = 0; i < 63; i++)
		//{
		//	std::cout << std::dec << i << " " << std::hex << theOS.m_Computer->m_Disk.read(i, 0).Contents << "\n";
		//}
		//
		//theOS.m_MMU.printValidFrames();
		*/
		/*
		//for (int i = 1; i <= 16; i++)
		//{
		//	printf("%4s %10s %5s %5s %5s\n", "Job", "Instruction", "Page", "Frame", "Index");
		//	PCB* pcb1 = theOS.m_PCB_Map.at(i);
		//	for (int i = 0; i < pcb1->totalSpaceInRAM(); i++)
		//		{
		//			theOS.m_MMU.ReadOrPageFault(i, pcb1);
		//		}
		//	theOS.m_MMU.printValidFrames();
		//}
		*/
		switch (SchedAlgor)
		{
		case 1: theOS.m_Scheduler.SchedType = FIFO;
			break;
		case 2: theOS.m_Scheduler.SchedType = SJF;
			break;
		case 3: theOS.m_Scheduler.SchedType = PRIORITY;
		}
		//theOS.m_MMU.ManageMemory = true;
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
							//writeQueue.push(theOS.m_ReadyQueue.front());
							theOS.m_ShortTerm.Dispatch(i);
							theOS.m_Computer->m_CPU[i].m_thread_ptr = new std::thread(&CPU::CPU_Run_thread, theOS.m_Computer->m_CPU[i]);
						}
					}
				}
				/*for (int c = 0; c < theComputer->GetNumCPUs(); ++c)
					if (theOS.m_Computer->m_CPU[c].m_thread_ptr->joinable())
						theOS.m_Computer->m_CPU[c].m_thread_ptr->join();*/
			}
		}

		//PCB* pcb1 = theOS.m_PCB_Map.at(1);
		//theOS.m_ReadyQueue.push(pcb1);
		//theOS.m_ShortTerm.Dispatch(0);
		//theOS.m_Computer->m_CPU[0].m_thread_ptr = new std::thread(&CPU::CPU_Run_thread, theOS.m_Computer->m_CPU[0]);


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
		if (coreDump == 'y')
		{
			printDisk(OS_Pointer);
		}
		std::cout << "Print Metrics? (y/n)";
		char metrics;
		std::cin >> metrics;
		if (metrics == 'y')
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
