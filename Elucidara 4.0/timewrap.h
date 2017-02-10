#pragma once
#include "stdafx.h"

/*	class: timewrap
This is a synchronization wrapper for multi-threaded applications.
If the value wrapped bu this object is requested by an outside object, this wrapper will force a
condition_variable::wait until the wrapped value doesn't points to null anymore
To prevent deadlocks the wrapped pointer CAN'T be set to null after that anymore
(thus the condition_variable::notify_all only happens once)

this wrapper was designed to control asynchronously the building process of a vulkan rendering engine.
since the objects never change once they are build this wraper works like a charm.
Author: Thiago A. Silva
*/

template<class resource>
class timewrap
{
private:
	//pointer to an object being wrapped by this class
	resource* _resource = nullptr;

	//synchronization objects 
	condition_variable _flag;
	once_flag _setFlag;

public:
	//mutex for the resource wrapped within this object
	mutex lock;

	//Wait until the data is filled, if the data is filled ignore the wait and keep going
	inline void wait() {

		//if there's no data being held
		if (!_resource) {

			//create a thread lock
			unique_lock<mutex> lock(lock);

			//wait til resource is filled with info
			_flag.wait(lock, [this]() { return _resource != nullptr; });
		}
	}

	//Set the information inside the wrapper. it can only be set once!
	inline void set(resource* value) {

		//set a lock-guard to protect the content of this wrapper
		lock_guard<mutex> lock(lock);

		//if the variable is not being set to null
		if (value) {

			//set the variable with the new value
			_resource = value;

			//fire event to proceed with all threads waiting for this object to come
			call_once(_setFlag, [this]() { _flag.notify_all(); });
		}
	}

	//get the resource for reading if this resource is null will wait until its not anymore
	//obs: i know i should make this return the object and not a pointer but doing so
	//would make this wrapper not work with SDL_Window objects...
	inline resource* get() {
		//set to wait
		wait();

		//when the wrapped object is not a nullptr anymore return it
		return _resource;
	}

	inline void destroy()
	{
		if (_resource)
			delete _resource;
	}
};
