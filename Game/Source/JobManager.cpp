#include "stdafx.h"
#include "JobManager.h"
#include "Worker.h"

bool JobManager::Shutdown = false;
std::mutex JobManager::lock;
std::vector<Job*> JobManager::jobs;
std::atomic<int> JobManager::jobcount = 0;

JobManager::JobManager(int numberOfWorkers)
{
	numWorkers = numberOfWorkers;
	for (int i = 0; i < numWorkers; i++)
	{
		workers.push_back(new Worker(i));
	}
}

JobManager::~JobManager()
{
	for (Worker* worker : workers)
	{
		delete worker;
	}
}

void JobManager::Begin()
{
	for (Worker* worker : workers)
	{
		worker->Begin();
	}
}

void JobManager::End()
{
	JobManager::Shutdown = true;
	for (Worker* worker : workers)
	{
		worker->End();
	}
}

void JobManager::AddJob(Job* job)
{
	JobManager::lock.lock();
	JobManager::jobs.push_back(job);
	JobManager::jobcount++;
	JobManager::lock.unlock();
}

void JobManager::WaitForJobs()
{
	while (!JobManager::jobcount == 0)
	{
		// do nothing
	}
}