#include "stdafx.h"

/*	class: ThreadPool
This is a async thread pool, its purpose is to process asynchronously stashed functions
this object is was implemented as a singleton.
To add a command use the static method add_command(delegate x), this will stash your delegate
and once the a thread is free it will process it.
hit Kill to make the threads exit their infinite cicly (killing them)
manually call start_cycle to put the current thread into a infinite cycle of delegate resolution
care when calling start_cycle through the main thread.
Author: Thiago A. Silva
*/

ThreadPool* ThreadPool::s = nullptr;

//stash commands to be executed later
void ThreadPool::add_command(function<void()>  command) {
	lock_guard<mutex> lock(s->queuelock);
	s->commandQueue.push(command);
	s->ready.notify_one();
}

//kill all the threads in the pool
void ThreadPool::kill() {
	s->run = false;
}

//set the thread to solve commands
void ThreadPool::start_cycle()
{
	//inialize a method pointer to load the stashed methods
	function<void()> function = nullptr;

	//start thread cycle
	while (s->run)
	{
		{
			//this lock ensures the thread safety to command queue 
			unique_lock<mutex> lock(s->queuelock);

			//if the queue is empty
			if (s->commandQueue.empty())

				//sleep til a command is stashed
				s->ready.wait(lock, []() {return !s->commandQueue.empty();});
		}
		{
			//this lock (again) ensures the thread safety to command queue 
			lock_guard<mutex> lock(s->queuelock);

			//save the top command
			function = s->commandQueue.front();

			//remove it from the top of the queue
			s->commandQueue.pop();
		}
		//execute it
		function();

		//delete it
		function = nullptr;
	}
}

//initialize x ammount of threads and put them into the infinite resolution cycle 
ThreadPool::ThreadPool(const int threadcount)
{
	s = this;
	for (size_t i = 0; i < threadcount; i++) {
		thread daemon(ThreadPool::start_cycle);
		daemon.detach();
	}
}

//kill all threads
ThreadPool::~ThreadPool()
{
	kill();
}
