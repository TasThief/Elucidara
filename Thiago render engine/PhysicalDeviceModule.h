#pragma once
#include "RenderEngine.h"
class Hub;

class PhysicalDeviceModule
{
public:
	int graphicsFamilyIndex = -1;
	int  presentFamilyIndex = -1;
	int  computeFamilyIndex = -1;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkPhysicalDeviceProperties properties;

	VkPhysicalDeviceFeatures deviceFeatures;

	PhysicalDeviceModule();

	~PhysicalDeviceModule();

	bool IsDeviceSuitable(VkPhysicalDevice device);

	vector<VkPhysicalDevice> GetDeviceList();

	void InitializePhysicalDevice();

	void FindQueueFamilies(VkPhysicalDevice device);

	
};

