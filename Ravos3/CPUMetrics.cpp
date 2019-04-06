#include "pch.h"

extern int TOTAL_WAIT_TIME;//used for overall averaging, not one CPU specific
extern int TOTAL_JOBS_EXECUTED;//used in metrics and CPUmetrics
extern int TOTAL_COMPLETION_TIME;//used for overall averaging, not one CPU specific 
//increments job count by one
bool CPUMetrics::updateJobCount()
{
	JobCount++;
	return true;
}
//adds job PID to JobsInThisCPU vector and increments the JobCount
bool CPUMetrics::updateJobsInThisCPU(int PID)
{
	JobsInThisCPU.push_back(PID);
	updateJobCount();
	return true;
}

void CPUMetrics::setWriteKey(bool keyVal)
{
	WriteKey = keyVal;
	WRITE_KEY = keyVal;//makes sure other processes dont write at the same time  
}


//returns vector JobsInThisCPU
std::vector<int> CPUMetrics::getWhichJobsInThisCPU()
{
	if (CPUMetrics::JobsInThisCPU.empty())
	{
		std::cout << "CPU has not processed any Jobs";
		return CPUMetrics::JobsInThisCPU;
	}
	return CPUMetrics::JobsInThisCPU;

}
//returns percentage of jobs in this CPU compared to all jobs executed
int CPUMetrics::getPercentOfJobsAssigned()
{
	percentOfJobs = (JobCount / TOTAL_JOBS_EXECUTED);
	return percentOfJobs;
}
