#pragma once
template<typename T>
class Deleter
{
public:
	Deleter() {
		object = new T();
	}

	~Deleter() {
		delete object;
		object = NULL;
	}

	T* operator &() {
		return object;
	}

	operator T() const {
		return *object;
	}
	T operator *(){
		return *object;
	}

	T* operator ->(){
		return object;
	}

	T* Reset() {
		delete object;
		object = NULL;
		return object;
	}

private:
	T* object;
};
