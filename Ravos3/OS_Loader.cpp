#include "pch.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>//for pow in the HexNumToInt

int HexNumToInt(std::string hexstr);
int assignPCB(PCB* tpcb, std::string info);
int assignDataBuffToPCB(PCB* tpcb, std::string info, int startIndex);

bool OS::Load(std::string filename)
{
	//PCB(int ID, int priority_Val, int status, int arr, int len)
	//int for current job

	int indexForInstruc = 0;//keeps track of the index in RAM
	int indexForData = 0;//keeps rack of index in DISK
	//std::ifstream in(Filename, ios::in);//opens file to be read
	std::ifstream infile(filename);
	std::string str;
	std::string instruct;
	std::string data;
	PCB *tempcb = new PCB();
	//Memory::MemoryWord instruc;
	while (std::getline(infile, str))
	{
		//std::getline(in, str, '\n');//(source, assigns to, delimiting char)
		if (str.find("JOB"))//if the string contains JOB
		{
			std::string jobInfo = str.substr(0, str.find("//JOB"));
			int jlen = assignPCB(tempcb, jobInfo);//parses and assigns info to PCB and returns length of instructions
			for (int i = 0; i < jlen; i++)
			{
				std::getline(infile, instruct, '\n');
				//std::istringstream iss2(instruct);
				MemoryWord k = MemoryWord(HexNumToInt(instruct));
				//MemoryWord k = MemoryWord(static_cast<int>(instruct));
				m_Computer->m_RAM.write((i + indexForInstruc), k);//writes to ram
			}
			indexForInstruc = jlen;//should be index for last instuction put in


		}
		if (str.find("DATA"))//if the string contains JOB
		{
			std::string dataBufInfo = str.substr(0, str.find("//DATA"));
			int blen = assignDataBuffToPCB(tempcb, dataBufInfo, indexForData);//parses and assigns info to PCB and returns length of instructions
			for (int j = 0; j < blen; j++)
			{
				std::getline(infile, data, '\n');//(source, destination, delimiter)
				//std::istringstream iss2(instruct);
				MemoryWord b = MemoryWord(HexNumToInt(data));
				m_Computer->m_Disk.write((j + indexForData), b);//writes to disk
			}


		}
	}
	return false;
}

//new hexnum to int using char*
int HexNumToInt(std::string hexstr) 
{
	//char *s = <char*>(hexstr.c_str());//cast to char not working??
	////converst string to long int
	//int decVal = (int)strtol(s, NULL, 0);
	//return decVal;
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << hexstr;
	ss >> x;
	return x;
}
//takes a string which is a hex number to decimal 
//int HexNumToInt(std::string hexstr)
//{
//	int digit;//wil be returned
//	for (int j = 0; j < hexstr.length(); j++)
//	{
//		char temp1 = hexstr[j];
//		if (isdigit(temp1))
//		{
//			int temp2 = atoi(temp1);
//			digit += (temp2)*pow(16.0, (hexstr.length() - j));
//		}
//		else
//	}
//	return digit;
//}

//parses the first line information //JOB  12 14 C and assigns to new PCB, returns length to be used 
int OS::assignPCB(PCB *tpcb, std::string info)
{
	std::istringstream iss(info);
	std::string token;
	int spaceKeeper = 0;
	int Id;//1
	int length;//2
	int prior;
	while (std::getline(iss, token, ' '))//source, asigns to, delimiter
	{
		if (spaceKeeper == 1)
		{
			Id = HexNumToInt(token);
			tpcb->process_ID = Id;
		}
		if (spaceKeeper == 2)
		{
			length = HexNumToInt(token);
			tpcb->ProgramSize = length;
		}
		else
		{
			prior = HexNumToInt(token);
			tpcb->priority = prior;
		}
		spaceKeeper++;
		//std::cout << token << std::endl;
	}
	//what is arr?
	return length;
	/*std::cout << Id << std::endl;
	std::cout << length << std::endl;
	std::cout << prior << std::endl;*/
}

int OS::assignDataBuffToPCB(PCB* tpcb, std::string info, int startIndex)
{
	std::istringstream iss(info);
	std::string token;
	int spaceKeeper = 0;
	int input;//1
	int output;//2
	int temp;
	int lengthOfAll;//length of all data
	while (std::getline(iss, token, ' '))//source, asigns to, delimiter
	{
		if (spaceKeeper == 1)
		{
			input = HexNumToInt(token);
			tpcb->InputBufferSize = input;
			tpcb->InputBufferStart = startIndex;
			//PCB::setInputBuff(startIndex, input);

		}
		if (spaceKeeper == 2)
		{
			output = HexNumToInt(token);
			tpcb->OutputBufferSize = input;
			tpcb->OutputBufferStart = (startIndex+input);
			//PCB::setOutputBuff((startIndex+input), output);
		}
		else
		{
			temp = HexNumToInt(token);
			tpcb->TempBufferSize = input;
			tpcb->TempBufferStart = (startIndex + input + output);
			//PCB::setTempBuff((startIndex + input + output), temp);
		}
		spaceKeeper++;
		//std::cout << token << std::endl;
	}
	lengthOfAll = input + output + temp;
	return lengthOfAll;
	/*std::cout << Id << std::endl;
	std::cout << length << std::endl;
	std::cout << prior << std::endl;*/
}
//// Just for testing
//void ReadProgString(Memory &m, const char *s)
//{
//	int size = strlen(s);
//	int Loc = 0;
//	for (int i = 0; i + 8 < size; i += 8)
//	{
//		//      if (s[i] == '\n') ++i;
//		MemoryWord w;
//		for (int j = 0; j < 8; ++j)
//			w.Contents[j] = s[i + j];
//
//		m.write(Loc++, w);
//	}
//
//}
//const char* Prog1 =
//"C050005C"
//"4B060000"
//"4B010000"
//"4B000000"
//"4F0A005C"
//"4F0D00DC"
//"4C0A0004"
//"C0BA0000"
//"42BD0000"
//"4C0D0004"
//"4C060001"
//"10658000"
//"56810018"
//"4B060000"
//"4F0900DC"
//"43970000"
//"05070000"
//"4C060001"
//"4C090004"
//"10658000"
//"5681003C"
//"C10000AC"
//"92000000";
///*
//// Data 14 C C
//0x0000000A
//0x00000006
//0x0000002C
//0x00000045
//0x00000001
//0x00000007
//0x00000000
//0x00000001
//0x00000005
//0x0000000A
//0x00000055
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//0x00000000
//// END
//*/
//void OS::TestLoad1()
//{
//	PCB *c = new PCB();
//	c->process_ID = 1;
//	c->priority = 2;
//	c->ProgramSize = 17;
//	//  pcb->InputSize  = ??
//	//  pcb->OutputSize = ??
//	//  pcb->TempSize    = ??
//	ReadProgString(m_Computer->m_RAM, Prog1);
//	// Something like this...
////  m_PCB.insert(c->getProcessID, c);
//}
