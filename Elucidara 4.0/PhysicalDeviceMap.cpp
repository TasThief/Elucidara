#include "stdafx.h"
#include "PhysicalDeviceMap.h"

void PhysicalDeviceMap::set_device(const PhysicalDevice newDevice)
{
	device = new PhysicalDevice(newDevice);

	//the family prorperties collected from the device
	vector<QueueFamilyProperties> properties = device->getQueueFamilyProperties();

	//for each key in the family map
	for (map<QFB, FamilyIndex*>::iterator itr = indexMap->begin(); itr != indexMap->end(); itr++)

		//seek for a prorperty block related to it's type
		for (size_t j = 0; j < properties.size(); j++)

			//when find it, if that family has queues in it
			if ((properties[j].queueCount > 0 && properties[j].queueFlags & itr->first)) {
				itr->second->index = j;
				itr->second->info = properties[j];
			}
}

FamilyIndex* PhysicalDeviceMap::get_familyIndex(const QFB type)
{
	if (type == QFB::eCompute  ||
		type == QFB::eGraphics ||
		type == QFB::eTransfer ||
		type == QFB::eSparseBinding)
		return (*indexMap)[type];
	else
		throw invalid_argument("indexes can only be one of the 4 available");
}

int PhysicalDeviceMap::get_availableSurfaceFormat(SurfaceKHR& surface)
{
	//Save available color format
	SurfaceFormatKHR idealFormat;
	idealFormat.colorSpace = ColorSpaceKHR::eSrgbNonlinear;
	idealFormat.format = Format::eB8G8R8A8Unorm;

	vector<SurfaceFormatKHR> availableFormats = device->getSurfaceFormatsKHR(surface);

	//if there's no preferencial pick one of my choice
	if (availableFormats.size() == 1 && availableFormats[0].format == Format::eUndefined)
	{
		availableFormat = idealFormat;
		return 6;
	}
	//if they have a list of available, seek for the one I want
	else 
		for each (SurfaceFormatKHR format in availableFormats)
			if (format == idealFormat)
			{
				availableFormat = idealFormat;
				return 5;
			}

	//if they dont have pick one at random
	availableFormat = availableFormats[0];
	return 1;
}

int PhysicalDeviceMap::get_availablePresentationMode(SurfaceKHR& surface)
{
	//pick presentation mode
	for each (PresentModeKHR mode in device->getSurfacePresentModesKHR(surface))
		if (mode == PresentModeKHR::eMailbox) {
			availablePresentMode = PresentModeKHR::eMailbox;
			return 6;
		}

	//if mailbox is not available use fifo...
	availablePresentMode = PresentModeKHR::eFifo;
	return 3;
}

int PhysicalDeviceMap::get_availableExtent(SurfaceKHR& surface)
{
	SurfaceCapabilitiesKHR capabilities = device->getSurfaceCapabilitiesKHR(surface);

	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		availableExtent = capabilities.currentExtent;
		return 5;
	}
	else
	{
		availableExtent = { WIDTH, HEIGHT };
		availableExtent.width = max(capabilities.minImageExtent.width, min(capabilities.maxImageExtent.width, availableExtent.width));
		availableExtent.height = max(capabilities.minImageExtent.height, min(capabilities.maxImageExtent.height, availableExtent.height));
		return 2;
	}
}

void PhysicalDeviceMap::get_swapchainConstraints(SurfaceKHR& surface)
{
	grade += get_availableSurfaceFormat(surface);
	grade += get_availablePresentationMode(surface);
	grade += get_availableExtent(surface);
}

bool PhysicalDeviceMap::check_fitness(map <QFB, vector<float>>& requestMap, SurfaceKHR& surface)
{
	for (map <QFB, vector<float>>::iterator itr = requestMap.begin(); itr != requestMap.end(); itr++)
		if (!get_familyIndex(itr->first)->validate(itr->second))
			return false;

	if (presentQueueIndex->index == -1)
		return false;

	return true;
}

void PhysicalDeviceMap::find_presentQueue(SurfaceKHR& surface)
{
	for (map<QFB, FamilyIndex*>::iterator itr = indexMap->begin(); itr != indexMap->end(); itr++){
		if (device->getSurfaceSupportKHR(itr->second->index, surface))
			presentQueueIndex = itr->second;
	}
}

PhysicalDeviceMap::PhysicalDeviceMap()
{
	indexMap = new map <QFB, FamilyIndex*>();

	//initialize the family map to have 4 keys, one for each family type
	(*indexMap)[QFB::eCompute] = new FamilyIndex();
	(*indexMap)[QFB::eGraphics] = new FamilyIndex();
	(*indexMap)[QFB::eSparseBinding] = new FamilyIndex();
	(*indexMap)[QFB::eTransfer] = new FamilyIndex();
}

void PhysicalDeviceMap::destroy()
{
	for (map<QFB, FamilyIndex*>::iterator itr = indexMap->begin(); itr != indexMap->end(); itr++)
	{
		delete itr->second;
	}

	delete indexMap;

	if (device)
	{
		delete device;
	}
}