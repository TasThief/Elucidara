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
//#define threadDebug

ThreadPool* ThreadPool::s = nullptr;

//stash commands to be executed later
void ThreadPool::add_command(function<void()>  command) {
	lock_guard<mutex> lock(s->queuelock);
	s->commandQueue.push(command);
	s->ready.notify_one();
}

//set the thread to solve commands
void ThreadPool::start_cycle(ThreadInfo* info)
{
	unique_lock<mutex> lock(s->queuelock);

	while (info->isRunning)
	{

		if (s->commandQueue.empty())
		{

#if defined threadDebug
			cout << "hibernating" << endl;
#endif

			info->isAwake = false;

			s->ready.wait(lock, []() {return !s->commandQueue.empty();});

			info->isAwake = true;

#if defined threadDebug
			cout << "waking" << endl;
#endif

		}
#if defined threadDebug
		else
		{
			cout << "no need for sleep" << endl;
		}
#endif

		info->process = s->commandQueue.front();

		s->commandQueue.pop();
		
		lock.unlock();

		info->process();

#if defined threadDebug
		cout << "process finished"<<endl;
#endif

		lock.lock();

		info->process = nullptr;
	}
	info->isAlive = false;

	info->aliveBlock.notify_all();
}

//initialize x ammount of threads and put them into the infinite resolution cycle 
ThreadPool::ThreadPool(int threadcount)
{
	s = this;
	
	for (size_t i = 0; i < threadcount; i++) {
		
		threadInfos.push_back(new ThreadInfo());
		threadInfos[i]->id = i;

		thread daemon(ThreadPool::start_cycle,threadInfos[i]);
		daemon.detach();
	}
}
ThreadPool::ThreadPool() {}
//kill all threads
ThreadPool::~ThreadPool()
{
	for (vector<ThreadInfo*>::iterator itr = threadInfos.begin(); itr != threadInfos.end(); itr++){
		unique_lock<mutex> lock((*itr)->aliveMutex);
		if ((*itr)->isAlive)
			(*itr)->aliveBlock.wait(lock, [itr]() { return !(*itr)->isAlive;});
		delete *itr;
	}
}



/*
//set the thread to solve commands
void ThreadPool::start_cycle(ThreadInfo info)
{
//start thread cycle
while (info.isRunning)
{
{
//this lock ensures the thread safety to command queue
unique_lock<mutex> lock(s->queuelock);

//if the queue is empty
if (s->commandQueue.empty())
{
info.isAwake = false;
//sleep til a command is stashed
s->ready.wait(lock, []() {return !s->commandQueue.empty();});

info.isAwake = true;
}
}
{
//this lock (again) ensures the thread safety to command queue
lock_guard<mutex> lock(s->queuelock);

//save the top command
info.process = s->commandQueue.front();

//remove it from the top of the queue
s->commandQueue.pop();
}
//execute it
info.process();

//delete it
info.process = nullptr;
}
info.isAlive = false;
}

*/