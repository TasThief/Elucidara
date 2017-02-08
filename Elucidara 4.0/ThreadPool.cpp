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

void ThreadPool::start_cycle(ThreadInfo* info)
{
	unique_lock<mutex> lock(s->queuelock);

	while (info->isRunning)
	{
		if(lock.owns_lock())
			lock.unlock();

		info->burn();

		lock.lock();

		if (s->commandQueue.empty())
		{
			info->isAwake = false;

			s->ready.wait(lock, [info]() {return !info->isRunning || !s->commandQueue.empty();});

			info->isAwake = true;
		}

		if (!lock.owns_lock())
			lock.lock();
		
		info->process = s->drawProcess();

		if (lock.owns_lock())
			lock.unlock();
	}

	info->isAlive = false;

	info->aliveBlock.notify_all();
}


function<void()> ThreadPool::drawProcess()
{
	function<void()> result = nullptr;

	if (commandQueue.size() > 0)
	{
		result = commandQueue.front();

		commandQueue.pop();
	}

	return result;
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

//kill all threads
ThreadPool::~ThreadPool()
{
	for (vector<ThreadInfo*>::iterator itr = threadInfos.begin(); itr != threadInfos.end(); itr++)
		(*itr)->isRunning = false;

	s->ready.notify_all();
	
	for (vector<ThreadInfo*>::iterator itr = threadInfos.begin(); itr != threadInfos.end(); itr++) {
		if ((*itr)->isAlive)
		{
			unique_lock<mutex> lock((*itr)->aliveMutex);
			cout << "thread " << (*itr)->id << " is still busy, waiting for it to finish its process" << endl;
			(*itr)->aliveBlock.wait(lock, [itr]() { return !(*itr)->isAlive;});
		}
		cout << "thread " << (*itr)->id << " destroyed" << endl;
		delete *itr;
	}
}