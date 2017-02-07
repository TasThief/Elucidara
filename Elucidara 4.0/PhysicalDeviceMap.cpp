#include "stdafx.h"
#include "PhysicalDeviceMap.h"

void PhysicalDeviceMap::set_device(const PhysicalDevice newDevice)
{
	device = new PhysicalDevice(newDevice);

	//the family prorperties collected from the device
	vector<vk::QueueFamilyProperties> properties = device->getQueueFamilyProperties();

	//for each key in the family map
	for (map<vk::QueueFlagBits, FamilyIndex*>::iterator itr = indexMap->begin(); itr != indexMap->end(); itr++)

		//seek for a prorperty block related to it's type
		for (size_t j = 0; j < properties.size(); j++)

			//when find it, if that family has queues in it
			if ((properties[j].queueCount > 0 && properties[j].queueFlags & itr->first)) {
				itr->second->index = j;
				itr->second->info = properties[j];
			}
}

FamilyIndex* PhysicalDeviceMap::get_familyIndex(const vk::QueueFlagBits type)
{
	if (type == vk::QueueFlagBits::eCompute ||
		type == vk::QueueFlagBits::eGraphics ||
		type == vk::QueueFlagBits::eTransfer ||
		type == vk::QueueFlagBits::eSparseBinding)
		return (*indexMap)[type];
	else
		throw invalid_argument("indexes can only be one of the 4 available");
}

bool PhysicalDeviceMap::check_fitness(map <vk::QueueFlagBits, FamilyRequest*>* requestMap, function<bool(PhysicalDevice*)> predicate = nullptr)
{
	for (map <vk::QueueFlagBits, FamilyRequest*>::iterator itr = requestMap->begin(); itr != requestMap->end(); itr++)
		if (!get_familyIndex(itr->first)->validate(itr->second))
			return false;

	if (predicate && !predicate(device))
		return false;

	return true;
}

PhysicalDeviceMap::PhysicalDeviceMap()
{
	indexMap = new map <vk::QueueFlagBits, FamilyIndex*>();

	//initialize the family map to have 4 keys, one for each family type
	(*indexMap)[vk::QueueFlagBits::eCompute] = new FamilyIndex();
	(*indexMap)[vk::QueueFlagBits::eGraphics] = new FamilyIndex();
	(*indexMap)[vk::QueueFlagBits::eSparseBinding] = new FamilyIndex();
	(*indexMap)[vk::QueueFlagBits::eTransfer] = new FamilyIndex();
}

PhysicalDeviceMap::~PhysicalDeviceMap()
{
	//for each key in the family map, destroy each one of them;
	for (map<vk::QueueFlagBits, FamilyIndex*>::iterator itr = indexMap->begin(); itr != indexMap->end(); itr++)
		delete itr->second;

	delete indexMap;

	if (device)
		delete device;
}