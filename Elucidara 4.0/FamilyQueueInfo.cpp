#include "stdafx.h"
#include "FamilyQueueInfo.h"

vk::DeviceQueueCreateInfo FamilyQueueInfo::build_queueCreateInfo()
{
	vk::DeviceQueueCreateInfo result;

	return result
		.setPQueuePriorities(&priority)
		.setQueueCount(requestedCount)
		.setQueueFamilyIndex(index);
}

bool FamilyQueueInfo::validate()
{
	bool result = true;

	if ((required) && (index == -1 || info->queueCount < requestedCount))
		result = false;

	return result;
}


FamilyQueueInfo* FamilyQueueInfo::set_required(const bool i)
{
	required = i;
	return this;
}

FamilyQueueInfo* FamilyQueueInfo::set_count(const int i)
{
	requestedCount = i;
	return this;
}

FamilyQueueInfo* FamilyQueueInfo::set_index(const int i)
{
	index = i;
	return this;
}

FamilyQueueInfo* FamilyQueueInfo::set_priority(const float i)
{
	priority = i;
	return this;
}

FamilyQueueInfo* FamilyQueueInfo::set_info(const vk::QueueFamilyProperties i)
{
	if (info != nullptr)
		delete info;

	info = new vk::QueueFamilyProperties(i);

	return this;
}


const vk::QueueFamilyProperties* FamilyQueueInfo::get_info()
{
	return info;
}

int FamilyQueueInfo::get_index()
{
	return index;
}


FamilyQueueInfo::FamilyQueueInfo()
{
	required = false;
	index = -1;
	requestedCount = 0;
	priority = 0.0f;
	info = nullptr;
}

FamilyQueueInfo::~FamilyQueueInfo()
{
	if (info != nullptr)
	{
		delete info;
		info = nullptr;
	}
}