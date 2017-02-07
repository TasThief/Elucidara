#pragma once

struct ThreadInfo {
	int id = 0;
	bool isRunning = true;
	bool isAlive = true;
	bool isAwake = true;
	
	function<void()> process = nullptr;
	condition_variable aliveBlock;
	mutex aliveMutex;

	ThreadInfo(){}
};

class ThreadPool
{
private:
	static ThreadPool* s;

	mutex queuelock;

	mutex coutmutex;

	condition_variable ready;

	queue<function<void()> > commandQueue;

	vector<ThreadInfo*> threadInfos;


public:

	static void add_command(function<void()>  command);

	static void start_cycle(ThreadInfo* info);

	ThreadPool(int threadcount);
	ThreadPool();
	~ThreadPool();

};

