// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include <cctype>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

//#include  <cstddef> //includes byte

#include "Memory.h"
#include "Metrics.h"
#include "PCB.h"
#include "MMU.h"
#include "CPUMetrics.h"
#include "CPU.h"
#include "ShortTermScheduler.h"
#include "Scheduler.h"
#include "Computer.h"
#include "OS.h" 

#endif //PCH_H
