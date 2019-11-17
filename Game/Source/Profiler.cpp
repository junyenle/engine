#include "stdafx.h"
#include "Profiler.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

Profiler* Profiler::Get()
{
	static Profiler p;
	return &p;
}

Profiler::Timer* Profiler::GetTimer(const std::string &name)
{
	std::unordered_map<std::string, Timer*>::iterator it = mTimers.find(name);
	if (it != mTimers.end())
	{
		return it->second;
	}
	Timer* timer = new Timer();
	timer->mName = name;
	mTimers.insert({ name, timer });
	return timer;
}

Profiler::Profiler()
{
	errno_t err = fopen_s(&chromeFile, "profile.json", "w");
	DbgAssert(!err, "Problem creating chrome trace file 'profile.json'");
	fprintf(chromeFile, "[");
}

void Profiler::ResetAll()
{
	for (auto entry : mTimers)
	{
		entry.second->Reset();
	}
}

Profiler::~Profiler()
{
	FILE* file;
	errno_t err = fopen_s(&file, "profile.txt", "w");
	if (!err)
	{
		fprintf(file, "name: avg (ms), max (ms)\n");
	}
	for (auto entry : mTimers)
	{
		Timer* timer = entry.second;
		if (!err)
		{
			fprintf(file, "%s:, %f, %f\n", timer->GetName().c_str(), timer->GetAvg_ms(), timer->GetMax_ms());
		}
		delete timer;
	}
	fclose(file);
	fprintf(chromeFile, chromeString.substr(0, chromeString.length() - 2).c_str());
	fprintf(chromeFile, "]");
	fclose(chromeFile);
}
