#include "pch.h"
#include <sstream>
#include <fstream>
#include <math.h>

OS::OS(Computer *theComputer, bool UseMMU) :m_Scheduler(this), m_ShortTerm(this), m_MMU(this, true)
{
	m_Computer = theComputer;
	
	for (int i = 0; i < 4; i++) 
	{
		m_Computer->m_CPU[i].m_MMU = &m_MMU;
	}
}

bool OS::Boot(std::string filename)
{
	if (!Load(filename)) return false;
	return true;
}

// Returns true if all jobs have been executed
bool OS::allJobsExecuted()
{
	return m_PCB_Map.size() == m_Terminated_PCBs.size();
}



//new hexnum to int using char*
int OS::HexNumToInt(std::string hexstr)
{
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << hexstr;
	ss >> x;
	return x;
}

//parses the first line information //JOB  12 14 C and assigns to new PCB, returns length to be used 
int OS::assignPCB(PCB *tpcb, std::string info)
{
	std::istringstream iss(info);
	std::string token;
	int spaceKeeper = 0;

	while (std::getline(iss, token, ' '))//source, asigns to, delimiter
	{
		if (spaceKeeper == 2)
		{
			tpcb->setProcessID(HexNumToInt(token));
		}
		if (spaceKeeper == 3)
		{
			tpcb->setProgramSize(HexNumToInt(token));
		}
		if(spaceKeeper == 4)
		{
			tpcb->setPriority(HexNumToInt(token));
		}
		spaceKeeper++;
		//std::cout << token << std::endl;
	}
	m_PCB_Map.insert(std::pair<int, PCB*>(tpcb->process_ID, tpcb));
	//print function of map 
	//std::map<int, PCB*>::iterator it = m_PCB_Map.begin();
	//
	//// Iterate over the map using Iterator till end.
	//while (it != m_PCB_Map.end())
	//{
	//	// Accessing KEY from element pointed by it.
	//	int word = it->first;

	//	// Accessing VALUE from element pointed by it.
	//	PCB* count = it->second;

	//	std::cout << word << " :: " << count->getProcessID() << std::endl;

	//	// Increment the Iterator to point to next entry
	//	it++;
	//}
	return tpcb->ProgramSize;
}

int OS::assignDataBuffToPCB(PCB* tpcb, std::string info, int startIndex)
{
	std::istringstream iss(info);
	std::string token;
	int spaceKeeper = 0;

	while (std::getline(iss, token, ' '))//source, asigns to, delimiter
	{
		if (spaceKeeper == 2)
		{
			tpcb->setInputBufferSize(HexNumToInt(token));
			tpcb->setInputBufferStart(startIndex);
		}
		if (spaceKeeper == 3)
		{
			tpcb->setOutputBufferSize(HexNumToInt(token));
			tpcb->setOutputBufferStart(startIndex + tpcb->OutputBufferSize);
		}
		if (spaceKeeper == 4)
		{
			tpcb->setTempBufferSize(HexNumToInt(token));
			tpcb->setTempBufferStart(startIndex + tpcb->InputBufferSize + tpcb->OutputBufferSize);
		}
		spaceKeeper++;
		//std::cout << token << std::endl;
	}
	return tpcb->InputBufferSize + tpcb->OutputBufferSize + tpcb->TempBufferSize;
}


bool OS::Load(std::string filename)
{

	int indexForInstruc = 0;//keeps track of the index in RAM
	int indexForData = 0;//keeps rack of index in DISK
	
	std::ifstream infile(filename);
	std::string str;
	int isJobLine;
	int isDataLine;
	int isEndLine;
	PCB *tempcb = new PCB();

	while (std::getline(infile, str))
	{
		isJobLine = str.find("JOB");//returns index where JOB is found
		isDataLine = str.find("Data");//returns index where Data is found
		isEndLine = str.find("END");
		
		if (isJobLine >= 0 )//if the string contains JOB
		{
			int jlen = assignPCB(tempcb, str);//parses and assigns info to PCB and returns length of instructions
			tempcb->StartIndexDisk = indexForData;
			for (int i = 0; i < jlen; i++)
			{
				std::getline(infile, str, '\n');
				MemoryWord k = MemoryWord(HexNumToInt(str));
				m_Computer->m_Disk.write(i, k, indexForData);
			}

			indexForData += jlen;
		}
		if (isDataLine >= 0 )//if the string contains DATA
		{
			int blen = assignDataBuffToPCB(tempcb, str, indexForData);//parses and assigns info to PCB and returns length of instructions
			for (int j = 0; j < blen; j++)
			{
				std::getline(infile, str, '\n');//(source, destination, delimiter)
				MemoryWord b = MemoryWord(HexNumToInt(str));
				m_Computer->m_Disk.write(j, b, indexForData);//writes to disk ****
			}
			indexForData += blen ;
			int newBlenDivisbleByFour;
			if (indexForData % 4 != 0) //takes 
			{
				newBlenDivisbleByFour = (4 - (indexForData % 4)) + indexForData;
				for (indexForData; indexForData < newBlenDivisbleByFour; indexForData++)
				{
					MemoryWord k = MemoryWord(0);
					m_Computer->m_Disk.write(indexForData, k, 0);
				}
			}
			tempcb->setPercentOfCachePerProcess();
			tempcb->setPercentofRAMPerProcess();
		}
		if (isEndLine >= 0) 
		{
			tempcb = new PCB();//creates new PCB with same name so new memory address as well
		}
	}
	delete tempcb;

	return true;
}



