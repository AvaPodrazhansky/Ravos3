#pragma once
//made like 
class CPUMetrics 
{
private:
	int CPUNum;
	bool WriteKey;
	int JobCount;//number of jobs assigned to a certain CPU
	std::vector<int> JobsInThisCPU;//holds PID of jobs in this CPU
	int percentOfJobs;

public:

	CPUMetrics() {}//keeps track of the CPU metrics

	//increments job count by one
	bool updateJobCount();

	//adds job PID to JobsInThisCPU vector and increments the JobCount
	bool updateJobsInThisCPU(int PID);

	void setWriteKey(bool keyVal);//only called in CPU.cpp when write is occuring
	
	//returns vector JobsInThisCPU
	std::vector<int> getWhichJobsInThisCPU();

	//returns percentage of jobs in this CPU compared to all jobs executed
	int getPercentOfJobsAssigned();
	


};
