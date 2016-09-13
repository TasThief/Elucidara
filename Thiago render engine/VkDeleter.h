#pragma once
#include <vulkan/vulkan.h>
#include <functional>
using namespace std;

template <typename T>
class VkDeleter {
public:

	VkDeleter() {}

	void New(const VkInstance& instance, function<void(VkInstance, T, VkAllocationCallbacks*)> deletef) {
		this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
	}

	void New(const VkDevice& device, function<void(VkDevice, T, VkAllocationCallbacks*)> deletef) {
		this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
	}

	void New(function<void(T, VkAllocationCallbacks*)> deletef) {
		this->deleter = [=](T obj) { deletef(obj, nullptr); };
	}

	~VkDeleter() {
		cleanup();
	}

	T* operator &() {
		return &object;
	}

	operator T() const {
		return object;
	}

	T* Reset()	{
		cleanup();
		return &object;
	}

private:
	T object{ VK_NULL_HANDLE };
	 function<void(T)> deleter;

	void cleanup() {
		if (object != VK_NULL_HANDLE) {
			deleter(object);
		}
		object = VK_NULL_HANDLE;
	}
};