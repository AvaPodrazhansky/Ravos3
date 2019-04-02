#include "pch.h"

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
}
//returns vector JobsInThisCPU
std::vector<int> CPUMetrics::getWhichJobsInThisCPU()
{
	if (JobsInThisCPU.empty())
	{
		//std::cout << "CPU has not processed any Jobs";
		return;
	}
	return JobsInThisCPU;

}
//returns percentage of jobs in this CPU compared to all jobs executed
int CPUMetrics::getPercentOfJobsAssigned()
{
	percentOfJobs = (TOTAL_JOBS_EXECUTED - JobCount) / 100;
	return percentOfJobs;
}
