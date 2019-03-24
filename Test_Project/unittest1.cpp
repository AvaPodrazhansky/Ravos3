#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Ravos3/pch.h"
#include "../Ravos3/Computer.h"
#include "../Ravos3/CPU.h"
#include "../Ravos3/Memory.h"
#include "../Ravos3/OS.h"
#include "../Ravos3/PCB.h"
#include "../Ravos3/Scheduler.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test_Project
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		std::string k[23] = // = std::string[23];
		{ "C050005C",
		"4B060000",
		"4B010000",
		"4B000000",
		"4F0A005C",
		"4F0D00DC",
		"4C0A0004",
		"C0BA0000",
		"42BD0000",
		"4C0D0004",
		"4C060001",
		"10658000",
		"56810018",
		"4B060000",
		"4F0900DC",
		"43970000",
		"05070000",
		"4C060001",
		"4C090004",
		"10658000",
		"5681003C",
		"C10000AC",
		"92000000" };


		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			//Computer c();
			//OS theOS(c);
			//c.m_CPU[0]

		}

	};
}