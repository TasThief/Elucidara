#pragma once
#include "FamilyQueueInfo.h"

class PhysicalDeviceMap
{
public:
	PhysicalDevice* device;

	//Map containing all the family indexes and other sorts of info
	map <vk::QueueFlagBits, FamilyQueueInfo*>* familyMap;
	
	void set_device(const PhysicalDevice device);

	PhysicalDeviceMap();

	~PhysicalDeviceMap();
};

