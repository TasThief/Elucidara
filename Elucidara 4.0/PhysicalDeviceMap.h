#pragma once
#include "FamilyInfo.h"

class PhysicalDeviceMap
{
private:
	map <vk::QueueFlagBits, FamilyIndex*>* indexMap;

public:
	PhysicalDevice* device;
	
	void set_device(const PhysicalDevice device);

	FamilyIndex * get_familyIndex(const vk::QueueFlagBits type);

	bool check_fitness(map<vk::QueueFlagBits, vector<float>>& requestMap, function<bool(PhysicalDevice*)> predicate);

	PhysicalDeviceMap();

	void destroy();
};

