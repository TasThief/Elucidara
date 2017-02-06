#include "stdafx.h"
#include "PhysicalDeviceMap.h"

PhysicalDeviceMap::PhysicalDeviceMap()
{
	familyMap = new map <vk::QueueFlagBits, FamilyQueueInfo*>();

	//initialize the family map to have 4 keys, one for each family type
	(*familyMap)[vk::QueueFlagBits::eCompute] = new FamilyQueueInfo();
	(*familyMap)[vk::QueueFlagBits::eGraphics] = new FamilyQueueInfo();
	(*familyMap)[vk::QueueFlagBits::eSparseBinding] = new FamilyQueueInfo();
	(*familyMap)[vk::QueueFlagBits::eTransfer] = new FamilyQueueInfo();
}


PhysicalDeviceMap::~PhysicalDeviceMap()
{
	//for each key in the family map, destroy each one of them;
	for (map<vk::QueueFlagBits, FamilyQueueInfo*>::iterator itr = familyMap->begin(); itr != familyMap->end(); itr++)
		delete itr->second;

	delete familyMap;

	if (device)
		delete device;
}


void PhysicalDeviceMap::set_device(const PhysicalDevice newDevice)
{
	device = new PhysicalDevice(newDevice);

	//the family prorperties collected from the device
	vector<vk::QueueFamilyProperties> properties = device->getQueueFamilyProperties();

	//for each key in the family map
	for (map<vk::QueueFlagBits, FamilyQueueInfo*>::iterator itr = familyMap->begin(); itr != familyMap->end(); itr++)

		//seek for a prorperty block related to it's type
		for (size_t j = 0; j < properties.size(); j++)

			//when find it, if that family has queues in it
			if ((properties[j].queueCount > 0 && properties[j].queueFlags & itr->first))

				//set the information
				itr->second
				->set_info(properties[j])
				->set_index(j);
}
