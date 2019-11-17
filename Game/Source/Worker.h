#pragma once
#include <thread>
class Worker
{
public:
	Worker(int id);
	~Worker();
	void Begin();
	void End();
	static void Loop();
	std::thread* activeThread;
	int id; // for debug
};

