#pragma once

class ThreadPool
{
private:
	//singleton instance
	static ThreadPool* s;

	//mutex for the command queue
	mutex queuelock;

	//switch to kill threads
	bool run = true;

	//flux controll condition
	condition_variable ready;

	//command queue
	queue<function<void()> > commandQueue;

public:

	//adds a command to be processed
	static void add_command(function<void()>  command);

	static void kill();

	static void start_cycle();

	ThreadPool(const int threadcount);

	~ThreadPool();
};

