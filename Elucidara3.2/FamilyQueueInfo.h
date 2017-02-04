#pragma once
#include "stdafx.h"

class FamilyQueueInfo
{
private:

	bool required;
	int requestedCount;
	int index;
	float priority;
	vk::QueueFamilyProperties* info = nullptr;

public:
	vk::DeviceQueueCreateInfo build_queueCreateInfo();
	
	FamilyQueueInfo* set_required(const bool i);
	FamilyQueueInfo* set_count(const int i);
	FamilyQueueInfo* set_index(const int i);
	FamilyQueueInfo* set_priority(const float i);
	FamilyQueueInfo* set_info(const vk::QueueFamilyProperties i);

	bool validate();
	int get_index();

	const vk::QueueFamilyProperties* get_info();

	FamilyQueueInfo();
	~FamilyQueueInfo();
};