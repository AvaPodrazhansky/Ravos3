#include "pch.h"

#include <sstream>
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



int HexNumToInt2(std::string hexstr)
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

void TestTheExecute()
{
	std::string k[23]; // = std::string[23];
	k[0] = "C050005C";
	k[1] = "4B060000";
	k[2] = "4B010000";
	k[3] = "4B000000";
	k[4] = "4F0A005C";
	k[5] = "4F0D00DC";
	k[6] = "4C0A0004";
	k[7] = "C0BA0000";
	k[8] = "42BD0000";
	k[9] = "4C0D0004";
	k[10] = "4C060001";
	k[11] = "10658000";
	k[12] = "56810018";
	k[13] = "4B060000";
	k[14] = "4F0900DC";
	k[15] = "43970000";
	k[16] = "05070000";
	k[17] = "4C060001";
	k[18] = "4C090004";
	k[19] = "10658000";
	k[20] = "5681003C";
	k[21] = "C10000AC";
	k[22] = "92000000";

	std::string d[44];
	for (int m = 0; m < 44; m++)
	{
		d[m] = "00000000";
	}
	d[0] = "0000000A";
	d[1] = "00000006";
	d[2] = "0000002C";
	d[3] = "00000045";
	d[4] = "00000001";
	d[5] = "00000007";
	d[6] = "00000000";
	d[7] = "00000001";
	d[8] = "00000005";
	d[9] = "0000000A";
	d[10] = "00000055";

	Computer *c = new Computer();
	
	for (int i = 0; i < 23; i++)
	{
		c->m_RAM.write(i, MemoryWord(HexNumToInt2(k[i])), 0);
	}

	for (int i = 0; i < 44; i++)
	{
		c->m_Disk.write(i, MemoryWord(HexNumToInt2(d[i])),0);
	}

	c->m_CPU[0].isExecuting = false;
	c->m_CPU[0].m_PC = c->m_RAM.read(0,0).Contents;

	for (int i = c->m_CPU[0].m_PC; i < 23; i++)
	{
		c->m_CPU[0].Execute();
	}


}