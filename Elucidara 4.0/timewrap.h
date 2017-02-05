#pragma once
#include "stdafx.h"
template<class resource>
class timewrap
{
private:
	resource* _resource = nullptr;
	condition_variable _flag;
	once_flag _setFlag;
	
public:
	mutex lock;

	//Wait until the data is filled, if the data is filled ignore the wait and keep going
	inline void wait() {

		//if this is closed
		if (!_resource) {

			//create a mutex to read res value
			unique_lock<mutex> lock(lock);

			//wait til res is filled with info
			_flag.wait(lock, [this]() { return _resource; });
		}
	}

	//Set the information inside the wrapper. it can only be set once!
	inline void set(resource* value) {

		//lock 
		lock_guard<mutex> lock(lock);

		//if i am not setting stuff to null
		if (value) {

			//set res's value
			_resource = value;

			//if i havent opened the locks yet
			call_once(_setFlag, [this]() { _flag.notify_all(); });
		}
	}

	//get the resource for reading ***DO NOT ALTER ITS VALUE*** use SET instead, setting the value through here will couse a deadlock
	inline resource* get(){ return _resource; }

	inline operator resource*() const { return _resource; }
};

