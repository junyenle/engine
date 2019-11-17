#pragma once
#include "Job.h"
#include <mutex>
#include <atomic>

class JobManager
{
public:
	JobManager(int numberOfWorkers);
	~JobManager();
	std::vector<class Worker*> workers;
	int numWorkers;
	void Begin();
	void End();
	void AddJob(class Job*);
	void WaitForJobs();
	static bool Shutdown;
	static std::mutex lock;
	static std::vector<Job*> jobs;
	static std::atomic<int> jobcount;
};

