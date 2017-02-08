#pragma once

struct flag
{
	condition_variable condition;
	bool wait = true;
	mutex _mutex;

	void go()
	{
		wait = false;
		condition.notify_all();
	}

	void sleep()
	{
		if (wait) {
			unique_lock<mutex> lock(_mutex);
			condition.wait(lock, [this]() {return !wait;});
		}
	}
};

struct ThreadInfo {
	int id = 0;
	bool isRunning = true;
	bool isAlive = true;
	bool isAwake = true;
	
	function<void()> process = nullptr;
	condition_variable aliveBlock;
	mutex aliveMutex;

	ThreadInfo(){}
	void burn()
	{
		if (process){
			process();
			process = nullptr;
		}
	}
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

	function<void()> drawProcess();

	ThreadPool(int threadcount);

	~ThreadPool();

};

