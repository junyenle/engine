#include "stdafx.h"
#include "Worker.h"
#include <thread>
#include <iostream>
#include "JobManager.h"

Worker::Worker(int wid)
{
	id = wid;
}


Worker::~Worker()
{
}


void Worker::Begin()
{
	activeThread = new std::thread(Loop);
}

void Worker::End()
{
	activeThread->join();
	// TODO: release anything we add
	delete activeThread;
}

void Worker::Loop()
{
	while (!JobManager::Shutdown)
	{
		Job* thisjob = NULL;
		JobManager::lock.lock();
		if (!JobManager::jobs.empty())
		{
			thisjob = JobManager::jobs.front();
			JobManager::jobs.erase(JobManager::jobs.begin());
		}
		JobManager::lock.unlock();
		if (thisjob != NULL)
		{
			thisjob->DoIt();
			JobManager::jobcount--;
			delete thisjob;
		}
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
	return;
}
